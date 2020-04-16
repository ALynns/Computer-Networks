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
		registerFrom.setTitle("账号注册");
		registerFrom.setSize(400,250);//只对顶级容器有效
		registerFrom.setDefaultCloseOperation(3);//窗体关闭时结束程序
		registerFrom.setLocationRelativeTo(null);//居中
		registerFrom.setResizable(false);
 
		//选择布局类型，定义流式布局的对象,并且设置每个组件之间相隔5cm
		java.awt.FlowLayout fl=new java.awt.FlowLayout(FlowLayout.CENTER,5,5);
		registerFrom.setLayout(fl);//设置顶级容器的布局为流式布局
		
		//设置格式大小
		java.awt.Dimension dim2=new java.awt.Dimension(70, 50);//标签的大小
		java.awt.Dimension dim3=new java.awt.Dimension(250, 30);//输入框的大小		
		java.awt.Dimension dim4=new java.awt.Dimension(100, 40);//按钮的大小	
		
		//添加“账号”标签
		JLabel labname=new JLabel();
		labname.setText("账号：");
		labname.setPreferredSize(dim2);
		registerFrom.add(labname);
		
		//添加账号输入框，并添加监控事件
		JTextField accountTextField=new JTextField();
		accountTextField.setPreferredSize(dim3);
		registerFrom.add(accountTextField);
		
		//添加“密码”标签
		JLabel labpassword=new JLabel();
		labpassword.setText("密码：");
		labpassword.setPreferredSize(dim2);
		registerFrom.add(labpassword);
		
		//添加密码输入框，并添加监控事件
		JPasswordField passwordField=new JPasswordField();
		passwordField.setPreferredSize(dim3);
		registerFrom.add(passwordField);

		//添加“确认密码”标签
		JLabel labpasswordconfirm=new JLabel();
		labpasswordconfirm.setText("确认密码：");
		labpasswordconfirm.setPreferredSize(dim2);
		registerFrom.add(labpasswordconfirm);
		
		//添加密码输入框，并添加监控事件
		JPasswordField passwordConfirmField=new JPasswordField();
		passwordConfirmField.setPreferredSize(dim3);
		registerFrom.add(passwordConfirmField);
		
		javax.swing.JButton registerConfirmButton=new javax.swing.JButton();
		registerConfirmButton.setText("确认注册");
		registerConfirmButton.setPreferredSize(dim4);
		registerFrom.add(registerConfirmButton);
		
		
		registerFrom.setVisible(true);
		
		RegisterConfirmListener registerConfirmListener=new RegisterConfirmListener(loginForm,registerFrom,accountTextField,passwordField,passwordConfirmField);
		//对当前窗体添加监听方法
		registerConfirmButton.addActionListener(registerConfirmListener);//监控按钮
		
	}
}
