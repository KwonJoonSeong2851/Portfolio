using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class SC_UI_Login : MonoBehaviour
{
    public TextMeshProUGUI t_stateText;
    public TMP_InputField input_userId;
    public TMP_InputField input_userPassword;
    public Button bt_login;
    public Toggle toggle_save;

    public List<TMP_InputField> inputList = new List<TMP_InputField>();

    private void Start()
    {
        bool toggleOn = PlayerPrefs.GetInt("Toggle") == 1 ? true : false;
        if (toggleOn == true)
        {
            toggle_save.isOn = true;
            input_userId.text = PlayerPrefs.GetString("UserID");
            input_userPassword.text = PlayerPrefs.GetString("UserPWD");
        }
        inputList[GetCurrentPos()].Select();
    }

    public void Update()
    {
        if(Input.GetKeyDown(KeyCode.Tab))
        {
            InputMoveNext();
        }

        if(Input.GetKeyDown(KeyCode.Return))
        {
            Login();
        }
    }

    private void InputMoveNext()
    {
        int pos = GetCurrentPos();
        if(pos < inputList.Count -1)
        {
            pos++;
            inputList[pos].Select();
        }else if (pos == inputList.Count -1)
        {
            inputList[0].Select();
        }
    }

    private int GetCurrentPos()
    {
        for(int i = 0; i < inputList.Count; ++i)
        {
            if(inputList[i].isFocused == true)
            {
                return i;
            }
        }

        return 0;
    }

    private void Initialize()
    {
        if (SC_NDG_Network.IsServerConnected)
        {
            SC_NDG_Network.network.RegisterReceiveNotification(PacketType.E_I_ANS_ID_PW, OnReceiveLogin);
        }
        else
        {
            Debug.Log("fail Connect  server");
        }
    }

    public void Login()
    {
        Initialize();

        PlayerPrefs.SetInt("Toggle",toggle_save.isOn == true ? 1 : 0);
        PlayerPrefs.SetString("UserID", input_userId.text);
        PlayerPrefs.SetString("UserPWD", input_userPassword.text);

        if (input_userId.text.Length < 6 || input_userPassword.text.Length < 6 || input_userId.text.Length > 15 || input_userPassword.text.Length > 15)
        {
            t_stateText.text = "아이디와 비밀번호는 6글자 이상, 15글자 이하 이어야 입니다.";
            return;
        }

        bt_login.interactable = false;
        t_stateText.text = "로그인 요청 중"; 
        PK_C_REQ_ID_PW packet = new PK_C_REQ_ID_PW();

        packet.userId = input_userId.text;
        packet.userPassword = input_userPassword.text;

        SC_NDG_Network.network.SendReliable(packet);

    }

    public void SucceedLogin()
    {
        SceneManager.LoadScene("SN_Title");
    }

    public void OnReceiveLogin(PacketType type,PacketInterface rowPacket)
    {
        SC_NDG_Network.network.UnregiseterReceiveNotification(PacketType.E_I_ANS_ID_PW);
        PK_I_ANS_ID_PW packet = (PK_I_ANS_ID_PW)rowPacket;

        Debug.Log("Login Result :" + packet.result + " , " + packet.accountId);

        if(packet.result == true)
        {
            SC_NDG_Network.localPlayerAccount = packet.accountId;
            SC_NDG_Network.Instance.TestGetUserInfo();
            StartCoroutine(ReceiveCheck());
            
        } else
        {
            t_stateText.text = "로그인에 실패했습니다. 아이디,비밀번호를 다시 확인하세요.";
            bt_login.interactable = true;
        }
    }

    IEnumerator ReceiveCheck()
    {
        while(true)
        {
            if (SC_NDG_Network.localPlayerInfo != null)
            {
                SucceedLogin();
                break;
            }

            yield return new WaitForFixedUpdate();
        }

        yield return null;
    }

}
