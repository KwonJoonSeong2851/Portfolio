using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System;
using DG.Tweening;

public class SC_UI_SingIn : MonoBehaviour
{
    public Canvas canvas_login;
    public TextMeshProUGUI txt_StateText;
    public TMP_InputField in_userId;
    public TMP_InputField in_userPwd;
    public TMP_InputField in_userConfirmPwd;
    public TMP_InputField in_userNickName;

    public List<TMP_InputField> inputList = new List<TMP_InputField>();
    private Network network;

    public TextMeshProUGUI txt_today;
    public TextMeshProUGUI txt_Nickname;

    private Util_EnumState.CharacterType charType;

    public Image img_char;
    public TextMeshProUGUI txt_charName;
    public TextMeshProUGUI txt_guardioName;

    [SerializeField]
    Sprite[] sp_char = new Sprite[3];

    [SerializeField]
    private Image img_Stamp;
    [SerializeField]
    private Image img_popup;
    [SerializeField]
    private Sprite sp_SuccessPopup;
    [SerializeField]
    private Sprite sp_FailedPopup;

    bool isSuccess = false;
    float closeTime = 2f;

    private bool Initialize()
    {
        network = SC_NDG_Network.Instance.GetNetwork();

        if (network != null)
        {
            network.RegisterReceiveNotification(PacketType.E_I_ANS_SIGNIN, OnReceivSignIn);
            return true;
        }
        else
        {
            Debug.Log("Connect  server fail");
        }

        return false;
    }

    private void OnEnable()
    {
        isSuccess = false;
        closeTime = 2f;
        img_Stamp.gameObject.SetActive(false);
        img_popup.gameObject.SetActive(false);

        txt_StateText.text = "";
        in_userId.text = "";
        in_userPwd.text = "";
        in_userConfirmPwd.text = "";
        in_userNickName.text = "";

        txt_today.text = "2003년 " + DateTime.Now.ToString("MM월 dd일");
        txt_Nickname.text = "플레이어이름";

        charType = Util_EnumState.CharacterType.Wolf;
        ChangeCharacterInfo();

        inputList[GetCurrentPos()].Select();
    }

    private void OnDisable()
    {
        Debug.Log("가입화면 종료됨");
        closeTime = 2f;
        isSuccess = false;
    }

    private void Update()
    {
        if (in_userNickName.text != "")
            txt_Nickname.text = in_userNickName.text;

        if (isSuccess)
        {
            if (Input.GetMouseButtonDown(0))
                closeSignIn();

            if (closeTime > 0)
                closeTime -= Time.deltaTime;
            else
                closeSignIn();
        }


        if (Input.GetKeyDown(KeyCode.Tab))
        {
            InputMoveNext();
        }

        if (Input.GetKeyDown(KeyCode.Return))
        {
            bt_SigIn();
        }

    }

    private void InputMoveNext()
    {
        int pos = GetCurrentPos();
        if (pos < inputList.Count - 1)
        {
            pos++;
            inputList[pos].Select();
        }
        else if (pos == inputList.Count - 1)
        {
            inputList[0].Select();
        }
    }

    private int GetCurrentPos()
    {
        for (int i = 0; i < inputList.Count; ++i)
        {
            if (inputList[i].isFocused == true)
            {
                return i;
            }
        }

        return 0;
    }

    public void bt_SigIn()
    {
        if(Initialize() == false)
        {
            return;
        }

        if(ConfirmPassword() == false)
        {
            return;
        }

        if (in_userId.text.Length < 6 || in_userPwd.text.Length < 6 || in_userId.text.Length > 15 || in_userPwd.text.Length > 15)
        {
            txt_StateText.text = "아이디와 비밀번호는 6글자 이상, 15글자 이하 이어야 입니다.";
            StartCoroutine(PopupSet(false, 0f));
            return;
        }

        if (in_userNickName.text.Length < 2 || in_userNickName.text.Length > 15)
        {
            txt_StateText.text = "닉네임은 2글자 이상, 15글자 이하 이어야 합니다.";
            StartCoroutine(PopupSet(false, 0f));
            return;
        }

        Debug.Log("ID Length:" + in_userId.text.Length + "  PWD Length:" + in_userPwd.text.Length + " NickName Length:"+in_userNickName.text.Length);

        PK_C_REQ_SIGNIN packet = new PK_C_REQ_SIGNIN();
        packet.userId = in_userId.text;
        packet.userPwd = in_userPwd.text;
        packet.userNickName = in_userNickName.text;

        network.SendReliable(packet);
    }

    public bool ConfirmPassword()
    {
        if (in_userPwd.text == in_userConfirmPwd.text)
        {
            return true;
        }
        else
        {
            txt_StateText.text = "비밀번호 재확인이 일치하지 않습니다.";
            StartCoroutine(PopupSet(false, 0f));
            return false;
        }

       
    }

    public void OnReceivSignIn(PacketType type,PacketInterface rowPacket)
    {
        network.UnregiseterReceiveNotification(PacketType.E_I_ANS_SIGNIN);
        PK_I_ANS_SIGNIN packet = (PK_I_ANS_SIGNIN)rowPacket;

        switch(packet.result)
        {
            case 0:
                //this.gameObject.SetActive(false);
                //canvas_login.gameObject.SetActive(true);

                img_Stamp.gameObject.SetActive(true);
                img_Stamp.rectTransform.localScale = new Vector3(1.3f, 1.3f, 1.3f);
                img_Stamp.rectTransform.DOScale(0.6f, 0.8f);

                StartCoroutine(PopupSet(true, 0.8f));

                break;

            case 1:
                txt_StateText.text = "가입에 실패하였습니다.\n<size=19>사유: 아이디가 중복 됩니다.</size>";
                StartCoroutine(PopupSet(false, 0f));
                break;

            case 2:
                txt_StateText.text = "가입에 실패하였습니다.\n<size=19>사유: 닉네임이 중복 됩니다.</size>";
                StartCoroutine(PopupSet(false, 0f));
                break;

        }
    }

    public void bt_characterChange(bool next)
    {
        if (next)
        {
            switch (charType)
            {
                case Util_EnumState.CharacterType.Wolf:
                    charType = Util_EnumState.CharacterType.Beetle;
                    break;
                case Util_EnumState.CharacterType.Beetle:
                    charType = Util_EnumState.CharacterType.Smokey;
                    break;
                case Util_EnumState.CharacterType.Smokey:
                    charType = Util_EnumState.CharacterType.Wolf;
                    break;
                default:
                    charType = Util_EnumState.CharacterType.Smokey;
                    break;
            }
        }
        else
        {
            switch (charType)
            {
                case Util_EnumState.CharacterType.Wolf:
                    charType = Util_EnumState.CharacterType.Smokey;
                    break;
                case Util_EnumState.CharacterType.Beetle:
                    charType = Util_EnumState.CharacterType.Wolf;
                    break;
                case Util_EnumState.CharacterType.Smokey:
                    charType = Util_EnumState.CharacterType.Beetle;
                    break;
                default:
                    charType = Util_EnumState.CharacterType.Smokey;
                    break;
            }
        }

        ChangeCharacterInfo();
    }

    void ChangeCharacterInfo()
    {
        switch(charType)
        {
            case Util_EnumState.CharacterType.Wolf:
                img_char.sprite = sp_char[(int)charType - 1];
                txt_charName.text = "강혁";
                txt_guardioName.text = "라이칸";
                break;
            case Util_EnumState.CharacterType.Beetle:
                img_char.sprite = sp_char[(int)charType - 1];
                txt_charName.text = "재희";
                txt_guardioName.text = "케프리";
                break;
            case Util_EnumState.CharacterType.Smokey:
                img_char.sprite = sp_char[(int)charType - 1];
                txt_charName.text = "린";
                txt_guardioName.text = "스모키";
                break;
            default:
                charType = Util_EnumState.CharacterType.Smokey;
                img_char.sprite = sp_char[(int)charType - 1];
                txt_charName.text = "린";
                txt_guardioName.text = "스모키";
                break;
        }
        PlayerPrefs.SetInt("CharacterType", (int)charType);
    }

    IEnumerator PopupSet(bool success, float time)
    {
        yield return new WaitForSeconds(time);

        if(success)
        {
            img_popup.sprite = sp_SuccessPopup;
            txt_StateText.text = "가입이 완료되었습니다.";

            img_popup.GetComponent<Button>().onClick.AddListener(closeSignIn);
            isSuccess = true;
        }
        else
        {
            img_popup.sprite = sp_FailedPopup;
            img_popup.GetComponent<Button>().onClick.AddListener(() => img_popup.gameObject.SetActive(false));
        }
        img_popup.gameObject.SetActive(true);
    }

    void closeSignIn()
    {
        img_popup.gameObject.SetActive(false);

        this.gameObject.SetActive(false);
        canvas_login.gameObject.SetActive(true);
    }
}
