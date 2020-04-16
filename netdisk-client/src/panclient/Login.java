package panclient;

import java.awt.Desktop;
import java.awt.FlowLayout;
import java.net.URI;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JPasswordField;

public class Login {
	public void showUI() {
		javax.swing.JFrame loginForm=new javax.swing.JFrame();
		loginForm.setTitle("���̵�½");
		loginForm.setSize(340,250);//ֻ�Զ���������Ч
		loginForm.setDefaultCloseOperation(3);//����ر�ʱ��������
		loginForm.setLocationRelativeTo(null);//����
		loginForm.setResizable(false);
 
		//ѡ�񲼾����ͣ�������ʽ���ֵĶ���,��������ÿ�����֮�����5cm
		java.awt.FlowLayout fl=new java.awt.FlowLayout(FlowLayout.CENTER,5,5);
		loginForm.setLayout(fl);//���ö��������Ĳ���Ϊ��ʽ����
		
		//���ø�ʽ��С
		java.awt.Dimension dim2=new java.awt.Dimension(50, 50);//��ǩ�Ĵ�С
		java.awt.Dimension dim3=new java.awt.Dimension(250, 30);//�����Ĵ�С		
		java.awt.Dimension dim4=new java.awt.Dimension(100, 40);//��ť�Ĵ�С	
		
		//��ӡ��˺š���ǩ
		JLabel labname=new JLabel();
		labname.setText("�˺ţ�");
		labname.setPreferredSize(dim2);
		loginForm.add(labname);
		
		//����˺�����򣬲���Ӽ���¼�
		JTextField accountTextField=new JTextField();
		accountTextField.setPreferredSize(dim3);
		loginForm.add(accountTextField);
		
		//��ӡ����롱��ǩ
		JLabel labpassword=new JLabel();
		labpassword.setText("���룺");
		labpassword.setPreferredSize(dim2);
		loginForm.add(labpassword);
		
		//�����������򣬲���Ӽ���¼�
		JPasswordField passwordField=new JPasswordField();
		passwordField.setPreferredSize(dim3);
		loginForm.add(passwordField);
		
		//���һ��button��ť
		javax.swing.JButton loginButton=new javax.swing.JButton();
		loginButton.setText("��¼");
		loginButton.setPreferredSize(dim4);
		loginForm.add(loginButton);
		
		javax.swing.JButton registerButton=new javax.swing.JButton();
		registerButton.setText("ע��");
		registerButton.setPreferredSize(dim4);
		loginForm.add(registerButton);
		
		
		loginForm.setVisible(true);
		
		//����ʵ������¼��ť������Ķ��󣬲��ѵ�¼�������˺ź����������Ķ��󴫸���
		LoginListener loginListener=new LoginListener(loginForm,accountTextField,passwordField);
		//�Ե�ǰ������Ӽ�������
		loginButton.addActionListener(loginListener);//��ذ�ť
		
		//����ʵ������¼��ť������Ķ��󣬲��ѵ�¼�������˺ź����������Ķ��󴫸���
		RegisterListener registerListener=new RegisterListener(loginForm);
		//�Ե�ǰ������Ӽ�������
		registerButton.addActionListener(registerListener);//��ذ�ť
	}
}
