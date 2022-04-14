

using System;

public static class PacketFactory 
{
    public static PacketInterface GetPacket(Int64 packetType)
    {
        switch((PacketType)packetType)
        {
            case PacketType.E_C_REQ_CHATTING: return new PK_C_REQ_CHATTING2();
            case PacketType.E_S_ANS_CHATTING: return new PK_S_ANS_CHATTING();
            case PacketType.E_S_ANS_TEST: return new PK_S_ANS_TEST();
            case PacketType.E_C_REQ_ID_PW: return new PK_C_REQ_ID_PW();
            case PacketType.E_I_ANS_ID_PW: return new PK_I_ANS_ID_PW();
            case PacketType.E_C_REQ_SIGNIN: return new PK_C_REQ_SIGNIN();
            case PacketType.E_I_ANS_SIGNIN: return new PK_I_ANS_SIGNIN();
            case PacketType.E_C_REQ_USERINFO: return new PK_C_REQ_USERINFO();
            case PacketType.E_I_ANS_USERINFO: return new PK_I_ANS_USERINFO();
            case PacketType.E_C_REQ_GAMERESULT: return new PK_C_REQ_GAMERESULT();
            default:
                break;
        }
        return null;
    }

}
