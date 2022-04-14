using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;
using Photon.Pun;
using System.Text;
using UnityEngine.SceneManagement;
using System;

public class SC_UI_Chatting : MonoBehaviour
{
    public Util_EnumState.ChatSession session = Util_EnumState.ChatSession.Room;

    public TextMeshProUGUI chatLog;
    public TMP_InputField inputField;
    public TextMeshProUGUI inputText;
    public ScrollRect scrollRect;

    public Image img_Enable;
    public Image img_Disable;

    private Network network;
   
    void Start()
    {
        Initialize();
    }

    void OnEnable()
    {
        chatLog.text = "";
    }

    void LateUpdate()
    {

        if (Input.GetKeyDown(KeyCode.Return))
        {
            inputField.Select();
        }

        if(inputField.isFocused)
        {
            img_Enable.enabled = true;
            img_Disable.enabled = false;
        }
        else
        {
            img_Enable.enabled = false;
            img_Disable.enabled = true;
        }
    }


    public void Initialize()
    {
        if (SC_NDG_Network.IsServerConnected == false)
        {
            Debug.LogWarning("NDG_Network Connection failed ");
            return;
        }

        chatLog.text = "";
        Debug.Log("채팅 초기화");

        network = SC_NDG_Network.Instance.GetNetwork();
        network.RegisterReceiveNotification(PacketType.E_S_ANS_CHATTING, OnReceiveChatting);


        inputField.onEndEdit.AddListener(InputFieldEndEdit);
        inputField.onSelect.AddListener(InputFieldOnEnable);
        inputField.onDeselect.AddListener(InputFieldOnDisable);
    }


    public void OnReceiveChatting(PacketType type, PacketInterface rowPacket)
    {
        PK_S_ANS_CHATTING packet = (PK_S_ANS_CHATTING)rowPacket;

        if (session != packet.session)
            return;

        if (packet.session == Util_EnumState.ChatSession.Room)
        {
            if (PhotonNetwork.CurrentRoom.Name != packet.roomNumber.ToString())
            {
                return;
            }
        }

        chatLog.text += "["+packet.m_id +"]:"+ packet.m_text + "\n";


        scrollRect.verticalNormalizedPosition = 0.0f;

    }


    public void SendChatting()
    {
        if (inputField.text != "")
        {

            PK_C_REQ_CHATTING2 data = new PK_C_REQ_CHATTING2();
            data.session = session;

            if (session == Util_EnumState.ChatSession.Room)
            data.roomNumber = Int32.Parse(PhotonNetwork.CurrentRoom.Name);

            data.m_id = SC_NDG_Network.localPlayerInfo?.nickName;
            data.m_text = inputField.text;

            network.SendReliable(data);


            inputField.text = "";
        }
    }


    public void InputFieldEndEdit(string text)
    {

        if (Input.GetKeyDown(KeyCode.Return))
        {
            SendChatting();
        }
        
    }

    public void InputFieldOnEnable(string text)
    {
        img_Enable.enabled = true;
        img_Disable.enabled = false;
    }

    public void InputFieldOnDisable(string text)
    {
        img_Enable.enabled = false;
        img_Disable.enabled = true;
    }

}
