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
	
	private javax.swing.JTextField accountTextField;//账号输入框对象
	private javax.swing.JPasswordField passwordField;//密码输入框对象
	
	private javax.swing.JFrame login;//定义一个窗体对象
	
	public LoginListener(javax.swing.JFrame login,javax.swing.JTextField jt,javax.swing.JPasswordField jp) {
		this.login=login;//获取登录界面
		this.accountTextField=jt;//获取登录界面中的账号输入框对象
		this.passwordField=jp;//获取登录界面中的密码输入框对象
	}
	
	public void actionPerformed(ActionEvent loginEvent) {
		
		MariadbOP loginDB=new MariadbOP();
		
		String userName=accountTextField.getText();
		String passWord=new String(passwordField.getPassword());
		String loginStr="";
		
		try {
			loginStr = "select * from UserManage where UserName='"+userName+"' and PassWord='"+CreateMD5.getMD5(passWord)+"'";
		} catch (Exception e1) {
			// TODO 自动生成的 catch 块
			e1.printStackTrace();
		}
		ResultSet result=loginDB.sqlOP(loginStr);
		
		if(result!=null){
			JOptionPane.showMessageDialog(null,"登陆成功！",null,JOptionPane.INFORMATION_MESSAGE);
			login.setVisible(false);
			UserInterface UI=new UserInterface(userName);
			UI.showUI();
		}
		else{
			JOptionPane.showMessageDialog(null,"用户名或密码错误！",null,JOptionPane.ERROR_MESSAGE);
		}
			
		
	}
}
