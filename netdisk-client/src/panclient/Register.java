package panclient;

import java.awt.FlowLayout;

import javax.swing.JLabel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

public class Register {
	
	private javax.swing.JFrame loginForm;
	
	public Register(javax.swing.JFrame login) {
		loginForm=login;
	}
	
	public void showUI() {
		javax.swing.JFrame registerFrom=new javax.swing.JFrame();
		registerFrom.setTitle("�˺�ע��");
		registerFrom.setSize(400,250);//ֻ�Զ���������Ч
		registerFrom.setDefaultCloseOperation(3);//����ر�ʱ��������
		registerFrom.setLocationRelativeTo(null);//����
		registerFrom.setResizable(false);
 
		//ѡ�񲼾����ͣ�������ʽ���ֵĶ���,��������ÿ�����֮�����5cm
		java.awt.FlowLayout fl=new java.awt.FlowLayout(FlowLayout.CENTER,5,5);
		registerFrom.setLayout(fl);//���ö��������Ĳ���Ϊ��ʽ����
		
		//���ø�ʽ��С
		java.awt.Dimension dim2=new java.awt.Dimension(70, 50);//��ǩ�Ĵ�С
		java.awt.Dimension dim3=new java.awt.Dimension(250, 30);//�����Ĵ�С		
		java.awt.Dimension dim4=new java.awt.Dimension(100, 40);//��ť�Ĵ�С	
		
		//��ӡ��˺š���ǩ
		JLabel labname=new JLabel();
		labname.setText("�˺ţ�");
		labname.setPreferredSize(dim2);
		registerFrom.add(labname);
		
		//����˺�����򣬲���Ӽ���¼�
		JTextField accountTextField=new JTextField();
		accountTextField.setPreferredSize(dim3);
		registerFrom.add(accountTextField);
		
		//��ӡ����롱��ǩ
		JLabel labpassword=new JLabel();
		labpassword.setText("���룺");
		labpassword.setPreferredSize(dim2);
		registerFrom.add(labpassword);
		
		//�����������򣬲���Ӽ���¼�
		JPasswordField passwordField=new JPasswordField();
		passwordField.setPreferredSize(dim3);
		registerFrom.add(passwordField);

		//��ӡ�ȷ�����롱��ǩ
		JLabel labpasswordconfirm=new JLabel();
		labpasswordconfirm.setText("ȷ�����룺");
		labpasswordconfirm.setPreferredSize(dim2);
		registerFrom.add(labpasswordconfirm);
		
		//�����������򣬲���Ӽ���¼�
		JPasswordField passwordConfirmField=new JPasswordField();
		passwordConfirmField.setPreferredSize(dim3);
		registerFrom.add(passwordConfirmField);
		
		javax.swing.JButton registerConfirmButton=new javax.swing.JButton();
		registerConfirmButton.setText("ȷ��ע��");
		registerConfirmButton.setPreferredSize(dim4);
		registerFrom.add(registerConfirmButton);
		
		
		registerFrom.setVisible(true);
		
		RegisterConfirmListener registerConfirmListener=new RegisterConfirmListener(loginForm,registerFrom,accountTextField,passwordField,passwordConfirmField);
		//�Ե�ǰ������Ӽ�������
		registerConfirmButton.addActionListener(registerConfirmListener);//��ذ�ť
		
	}
}
