package panclient;

import java.awt.event.ActionListener;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import javax.swing.JOptionPane;

import com.ndktools.javamd5.Mademd5;

import databaseconnect.*;
import tools.*;

import java.awt.event.ActionEvent;

public class LoginListener implements ActionListener{
	
	private javax.swing.JTextField accountTextField;//�˺���������
	private javax.swing.JPasswordField passwordField;//������������
	
	private javax.swing.JFrame login;//����һ���������
	
	public LoginListener(javax.swing.JFrame login,javax.swing.JTextField jt,javax.swing.JPasswordField jp) {
		this.login=login;//��ȡ��¼����
		this.accountTextField=jt;//��ȡ��¼�����е��˺���������
		this.passwordField=jp;//��ȡ��¼�����е�������������
	}
	
	public void actionPerformed(ActionEvent loginEvent) {
		
		MariadbOP loginDB=new MariadbOP();
		
		String userName=accountTextField.getText();
		String passWord=new String(passwordField.getPassword());
		String loginStr="";
		
		try {
			loginStr = "select * from UserManage where UserName='"+userName+"' and PassWord='"+CreateMD5.getMD5(passWord)+"'";
		} catch (Exception e1) {
			// TODO �Զ����ɵ� catch ��
			e1.printStackTrace();
		}
		ResultSet result=loginDB.sqlOP(loginStr);
		
		if(result!=null){
			JOptionPane.showMessageDialog(null,"��½�ɹ���",null,JOptionPane.INFORMATION_MESSAGE);
			login.setVisible(false);
			UserInterface UI=new UserInterface(userName);
			UI.showUI();
		}
		else{
			JOptionPane.showMessageDialog(null,"�û������������",null,JOptionPane.ERROR_MESSAGE);
		}
			
		
	}
}
