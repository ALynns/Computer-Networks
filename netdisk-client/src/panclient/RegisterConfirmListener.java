package panclient;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.ResultSet;
import java.sql.SQLException;

import javax.swing.JOptionPane;

import databaseconnect.MariadbOP;
import tools.CreateMD5;

public class RegisterConfirmListener implements ActionListener{
	private javax.swing.JTextField accountTextField;//账号输入框对象
	private javax.swing.JPasswordField passwordField;//密码输入框对象
	private javax.swing.JPasswordField passwordConfirmField;//密码输入框对象
	
	private javax.swing.JFrame registerForm;//定义一个窗体对象
	private javax.swing.JFrame loginForm;//定义一个窗体对象
	
	
	public RegisterConfirmListener(javax.swing.JFrame login,javax.swing.JFrame register,javax.swing.JTextField account,javax.swing.JPasswordField password,javax.swing.JPasswordField passwordConfirm) {
		loginForm=login;
		registerForm=register;
		accountTextField=account;
		passwordField=password;
		passwordConfirmField=passwordConfirm;
	}
	
	public void actionPerformed(ActionEvent panButtonEvent) {
		
		String username=accountTextField.getText();
		String password=new String(passwordField.getPassword());
		String passwordConfirm=new String(passwordConfirmField.getPassword());
		
		if(username.isEmpty()) {
			JOptionPane.showMessageDialog(null,"用户名不能为空！",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		if(password.isEmpty()) {
			JOptionPane.showMessageDialog(null,"密码不能为空！",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		if(passwordConfirm.isEmpty()) {
			JOptionPane.showMessageDialog(null,"确认密码不能为空！",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		if(!password.equals(passwordConfirm)) {
			JOptionPane.showMessageDialog(null,"两次输入密码不一致！",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		if(checkPassword(password)!=1) {
			JOptionPane.showMessageDialog(null,"密码过于简单！请重新创建！",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		
		MariadbOP registerDB=new MariadbOP();
		String registerStr="";
		
		try {
			registerStr = "select * from UserManage where UserName='"+username+"'";
		} catch (Exception e1) {
			// TODO 自动生成的 catch 块
			e1.printStackTrace();
		}
		ResultSet result=registerDB.sqlOP(registerStr);
		try {
			if(result.next()){
				JOptionPane.showMessageDialog(null,"该用户名已被注册！请重新创建！",null,JOptionPane.ERROR_MESSAGE);
				registerDB.close();
			}
			else {
				
				try {
					registerStr="insert into UserManage set UserName='"+username+"',PassWord='"+CreateMD5.getMD5(password)+"',UserDir='"+username+"'";
					registerDB.sqlOP(registerStr);
					registerStr="create table "+username+"(FileKey int auto_increment primary key,FileName varchar(512) not null,FileType int default 0 not null,FileStatus int default 0 not null,P_Dir int not null,RealFile int)";
					registerDB.sqlOP(registerStr);
					registerStr="insert into "+username+" set FileName='root',FileType=0,FileStatus=0,P_Dir=0";
					registerDB.sqlOP(registerStr);
				} catch (Exception e) {
					// TODO 自动生成的 catch 块
					e.printStackTrace();
				}
				JOptionPane.showMessageDialog(null,"注册成功！",null,JOptionPane.INFORMATION_MESSAGE);
				registerDB.close();
				registerForm.setVisible(false);
				loginForm.setVisible(true);
			}
		} catch (SQLException e) {
			// TODO 自动生成的 catch 块
			e.printStackTrace();
		}
			
	}
	
	public int checkPassword(String passwordStr) {
		
		String regexN = "\\d{0,}";
		String regexLL = "[a-z]{0,}";
		String regexCL = "[A-Z]{0,}";
		String regexS = "[\\W_]{0,}";
		
		String regexNLL = "[\\da-z]{0,}";
		String regexNCL = "[\\dA-Z]{0,}";
		String regexNS = "[\\d\\W_]{0,}";
		String regexCSL = "[A-Za-z]{0,}";
		String regexLLS = "[a-z\\W_]{0,}";
		String regexCLS = "[A-Z\\W_]{0,}";
		
		String regexNCLL = "[\\dA-Za-z]{12,}";
		String regexNLLS = "[\\da-z\\W_]{12,}";
		String regexNCLS = "[\\dA-Z\\W_]{12,}";
		String regexLCLS = "[a-zA-Z\\W_]{12,}";
		
		String regexNCLLS = "[\\S]{12,}";
		
		

		if(passwordStr.matches(regexN)||passwordStr.matches(regexLL)||passwordStr.matches(regexCL)||passwordStr.matches(regexS)) {
			//System.out.println(passwordStr.matches(regexZST));
			return 0;
		}
		
		if(passwordStr.matches(regexNLL)||passwordStr.matches(regexNCL)||passwordStr.matches(regexNS)||passwordStr.matches(regexCSL)||passwordStr.matches(regexLLS)||passwordStr.matches(regexCLS)) {
			return 0;
		}
		
		if(passwordStr.matches(regexNCLL)||passwordStr.matches(regexNLLS)||passwordStr.matches(regexNCLS)||passwordStr.matches(regexLCLS)||passwordStr.matches(regexNCLLS)) {
			return 1;
		}
		
		return 0;
	}
}
