package panclient;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.ResultSet;
import java.sql.SQLException;

import javax.swing.JOptionPane;

import databaseconnect.MariadbOP;
import tools.CreateMD5;

public class RegisterConfirmListener implements ActionListener{
	private javax.swing.JTextField accountTextField;//�˺���������
	private javax.swing.JPasswordField passwordField;//������������
	private javax.swing.JPasswordField passwordConfirmField;//������������
	
	private javax.swing.JFrame registerForm;//����һ���������
	private javax.swing.JFrame loginForm;//����һ���������
	
	
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
			JOptionPane.showMessageDialog(null,"�û�������Ϊ�գ�",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		if(password.isEmpty()) {
			JOptionPane.showMessageDialog(null,"���벻��Ϊ�գ�",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		if(passwordConfirm.isEmpty()) {
			JOptionPane.showMessageDialog(null,"ȷ�����벻��Ϊ�գ�",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		if(!password.equals(passwordConfirm)) {
			JOptionPane.showMessageDialog(null,"�����������벻һ�£�",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		if(checkPassword(password)!=1) {
			JOptionPane.showMessageDialog(null,"������ڼ򵥣������´�����",null,JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		
		MariadbOP registerDB=new MariadbOP();
		String registerStr="";
		
		try {
			registerStr = "select * from UserManage where UserName='"+username+"'";
		} catch (Exception e1) {
			// TODO �Զ����ɵ� catch ��
			e1.printStackTrace();
		}
		ResultSet result=registerDB.sqlOP(registerStr);
		try {
			if(result.next()){
				JOptionPane.showMessageDialog(null,"���û����ѱ�ע�ᣡ�����´�����",null,JOptionPane.ERROR_MESSAGE);
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
					// TODO �Զ����ɵ� catch ��
					e.printStackTrace();
				}
				JOptionPane.showMessageDialog(null,"ע��ɹ���",null,JOptionPane.INFORMATION_MESSAGE);
				registerDB.close();
				registerForm.setVisible(false);
				loginForm.setVisible(true);
			}
		} catch (SQLException e) {
			// TODO �Զ����ɵ� catch ��
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
