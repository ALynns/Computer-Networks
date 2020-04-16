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
		loginForm.setTitle("网盘登陆");
		loginForm.setSize(340,250);//只对顶级容器有效
		loginForm.setDefaultCloseOperation(3);//窗体关闭时结束程序
		loginForm.setLocationRelativeTo(null);//居中
		loginForm.setResizable(false);
 
		//选择布局类型，定义流式布局的对象,并且设置每个组件之间相隔5cm
		java.awt.FlowLayout fl=new java.awt.FlowLayout(FlowLayout.CENTER,5,5);
		loginForm.setLayout(fl);//设置顶级容器的布局为流式布局
		
		//设置格式大小
		java.awt.Dimension dim2=new java.awt.Dimension(50, 50);//标签的大小
		java.awt.Dimension dim3=new java.awt.Dimension(250, 30);//输入框的大小		
		java.awt.Dimension dim4=new java.awt.Dimension(100, 40);//按钮的大小	
		
		//添加“账号”标签
		JLabel labname=new JLabel();
		labname.setText("账号：");
		labname.setPreferredSize(dim2);
		loginForm.add(labname);
		
		//添加账号输入框，并添加监控事件
		JTextField accountTextField=new JTextField();
		accountTextField.setPreferredSize(dim3);
		loginForm.add(accountTextField);
		
		//添加“密码”标签
		JLabel labpassword=new JLabel();
		labpassword.setText("密码：");
		labpassword.setPreferredSize(dim2);
		loginForm.add(labpassword);
		
		//添加密码输入框，并添加监控事件
		JPasswordField passwordField=new JPasswordField();
		passwordField.setPreferredSize(dim3);
		loginForm.add(passwordField);
		
		//添加一个button按钮
		javax.swing.JButton loginButton=new javax.swing.JButton();
		loginButton.setText("登录");
		loginButton.setPreferredSize(dim4);
		loginForm.add(loginButton);
		
		javax.swing.JButton registerButton=new javax.swing.JButton();
		registerButton.setText("注册");
		registerButton.setPreferredSize(dim4);
		loginForm.add(registerButton);
		
		
		loginForm.setVisible(true);
		
		//首先实例化登录按钮监听类的对象，并把登录界面中账号和密码输入框的对象传给它
		LoginListener loginListener=new LoginListener(loginForm,accountTextField,passwordField);
		//对当前窗体添加监听方法
		loginButton.addActionListener(loginListener);//监控按钮
		
		//首先实例化登录按钮监听类的对象，并把登录界面中账号和密码输入框的对象传给它
		RegisterListener registerListener=new RegisterListener(loginForm);
		//对当前窗体添加监听方法
		registerButton.addActionListener(registerListener);//监控按钮
	}
}
