package panclient;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import tools.BrowseWake;

public class RegisterListener implements ActionListener{
	
	private javax.swing.JFrame login;//定义一个窗体对象
	
	public RegisterListener(javax.swing.JFrame loginForm) {
		this.login=loginForm;
	}

	
	public void actionPerformed(ActionEvent registerEvent) {
		login.setVisible(false);
		Register registerForm=new Register(login);
		registerForm.showUI();
	}
}
