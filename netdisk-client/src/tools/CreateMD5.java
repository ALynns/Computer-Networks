package tools;

import java.security.MessageDigest;

public class CreateMD5 {
	public static String getMD5(String str) throws Exception {
        /** ����MD5���ܶ��� */
        MessageDigest md5 = MessageDigest.getInstance("MD5"); 
        /** ���м��� */
        md5.update(str.getBytes("GBK"));
        /** ��ȡ���ܺ���ֽ����� */
        byte[] md5Bytes = md5.digest();
        String res = "";
        for (int i = 0; i < md5Bytes.length; i++){
            int temp = md5Bytes[i] & 0xFF;
            if (temp <= 0XF){ // ת����ʮ�����Ʋ�����λ��ǰ�����
                res += "0";
            }
            res += Integer.toHexString(temp);
        }
        return res;
    }
}
