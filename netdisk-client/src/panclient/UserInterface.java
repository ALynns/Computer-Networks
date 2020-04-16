package panclient;

import java.awt.FlowLayout;
import java.awt.HeadlessException;
import java.awt.MenuItem;
import java.awt.PopupMenu;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.JTextArea;
import javax.swing.JTree;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;

import databaseconnect.MariadbOP;

public class UserInterface extends JTextArea implements MouseListener {
	private String username;
	private MariadbOP dbop;
	
	private javax.swing.JFrame uf;
	
	private DefaultTreeModel dt;
	private JTree tree;
	
	private ArrayList<TreePath> clipBoard;
	private int copyFlag;
	
	private DefaultMutableTreeNode root;
	private DefaultMutableTreeNode selectFile;
	private DefaultMutableTreeNode selectPFile;
	
	
	public UserInterface(String name){
		this.username=name;
		this.dbop=new MariadbOP();
		clipBoard=new ArrayList<TreePath>();
	}
	
	public void showUI() {
		uf=new javax.swing.JFrame();
		uf.setTitle("我的网盘["+username+"]");
		uf.setSize(700,600);//只对顶级容器有效
		uf.setDefaultCloseOperation(3);//窗体关闭时结束程序
		uf.setLocationRelativeTo(null);//居中
		uf.setResizable(false);
		uf.setVisible(true);
		
		java.awt.FlowLayout fl=new java.awt.FlowLayout(FlowLayout.CENTER,200,30);
		uf.setLayout(fl);//设置顶级容器的布局为流式布局
		
		java.awt.Dimension dimLabel=new java.awt.Dimension(300, 50);
		java.awt.Dimension dimButton=new java.awt.Dimension(120, 40);//按钮的大小	
		
		javax.swing.JButton panButton=new javax.swing.JButton();
		panButton.setText("上传本地文件");
		panButton.setPreferredSize(dimButton);
		uf.add(panButton);
		
		javax.swing.JButton localButton=new javax.swing.JButton();
		localButton.setText("下载云盘文件");
		localButton.setPreferredSize(dimButton);
		uf.add(localButton);
		
		String opStr="select * from "+username+" where P_Dir=0";
		ResultSet result=dbop.sqlOP(opStr);
		int P_Dir=0;
		String fileName="";
		int dir=0;


		try {
			while(result.next()) {
				opStr="select * from "+username+" where P_Dir=0";
				
				dir=result.getInt("FileKey");
				fileName=result.getString("FIleName");
				P_Dir=result.getInt("P_Dir");
				
			}
		} catch (SQLException e) {
			// TODO 自动生成的 catch 块
			e.printStackTrace();
		}
		root=treeUI(1,fileName,0,0);
		dt=new DefaultTreeModel(root);
		tree = new JTree(dt);
		tree.addMouseListener(this);
		uf.add(tree);
		
		//首先实例化登录按钮监听类的对象，并把登录界面中账号和密码输入框的对象传给它
		PanButtonListener panButtonListener=new PanButtonListener(dt,root,selectFile,selectPFile);
		//对当前窗体添加监听方法
		panButton.addActionListener(panButtonListener);//监控按钮
		
	}
	
	private DefaultMutableTreeNode treeUI(int dir,String p_filename,int filetype,int P_Dir) {

		String opStr="";
		ResultSet result=null;
		
		if(filetype==1) {
			TreeNode node=new TreeNode(p_filename,dir,filetype,P_Dir);
			DefaultMutableTreeNode group=new DefaultMutableTreeNode(node);
			return group;
		}

		TreeNode node=new TreeNode(p_filename,dir,filetype,P_Dir);
		DefaultMutableTreeNode group=new DefaultMutableTreeNode(node);
		
		
		try {
			opStr="select * from "+username+" where P_Dir="+String.valueOf(dir);
			result=dbop.sqlOP(opStr);
			if(!result.next()) {
				group.add(new DefaultMutableTreeNode(new TreeNode("<空目录>",-1,-1,dir)));
				return group;
			}
			else {
				int fileID=result.getInt("FileKey");
				String fileName=result.getString("FileName");
				int fileType=result.getInt("FileType");
				
				group.add(treeUI(fileID,fileName,fileType,dir));
			}
			
			while(result.next()) {
				int fileID=result.getInt("FileKey");
				String fileName=result.getString("FileName");
				int fileType=result.getInt("FileType");
				
				group.add(treeUI(fileID,fileName,fileType,dir));
			}
		} catch (SQLException e) {
			// TODO 自动生成的 catch 块
			e.printStackTrace();
		}
		
		return group;
	}
	
	private JPopupMenu setRightMouseMenu(DefaultMutableTreeNode node,DefaultMutableTreeNode pNode ,TreePath path,TreePath []paths) {
		JPopupMenu menu=new JPopupMenu();
		
		JMenuItem menuItem1 = new JMenuItem("新建目录");//新建目录
		JMenuItem menuItem2 = new JMenuItem("复制");//复制
		JMenuItem menuItem3 = new JMenuItem("粘贴");//粘贴
		JMenuItem menuItem4 = new JMenuItem("删除");//删除
		//JMenuItem menuItem5 = new JMenuItem("全选");//全选
		JMenuItem menuItem6 = new JMenuItem("剪切");//剪切
		
		menuItem1.setEnabled(true);
		menuItem2.setEnabled(true);
		menuItem3.setEnabled(true);
		menuItem4.setEnabled(true);
		//menuItem5.setEnabled(true);
		
		
		if(((TreeNode)node.getUserObject()).getFileType()==-1) 
			menuItem2.setEnabled(false);
			
		if(clipBoard.size()==0||paths.length>0)
			menuItem3.setEnabled(false);
			
		if(((TreeNode)node.getUserObject()).getFileType()==-1)
			menuItem4.setEnabled(false);
		
		if(((TreeNode)node.getUserObject()).getFileType()==-1)
			menuItem6.setEnabled(false);
	
		
		/*menuItem1.setLabel("新建目录");
		menuItem2.setLabel("复制");
		menuItem3.setLabel("粘贴");
		menuItem4.setLabel("删除");
		menuItem5.setLabel("全选");*/
		
		menuItem1.addActionListener(new java.awt.event.ActionListener() { //新建目录
			public void actionPerformed(ActionEvent e) {
				
				String opStr="";
				ResultSet result=null;
				
				String dirName;
				dirName=JOptionPane.showInputDialog(null,"请输入目录名：\n","title",JOptionPane.PLAIN_MESSAGE); 
				
				if(dirName=="")
					return;
				
				if(((TreeNode)node.getUserObject()).getFileType()==-1||((TreeNode)node.getUserObject()).getFileType()==1) {//空目录
					try {
						
						try {
							opStr="select * from "+username+" where FileName='"+dirName+"' and FileType=0 and P_Dir="+((TreeNode)node.getUserObject()).getP_FileKey();
							result=dbop.sqlOP(opStr);
							if(result.next()) {
								JOptionPane.showMessageDialog(null,"该目录名已存在！",null,JOptionPane.ERROR_MESSAGE);
								return;
							}
						} catch (SQLException e1) {
							// TODO 自动生成的 catch 块
							e1.printStackTrace();
						}
						
						opStr="insert into "+username+" set FileName='"+dirName+"',FileType=0,P_Dir="+((TreeNode)node.getUserObject()).getP_FileKey();
						dbop.sqlOP(opStr);
						opStr="select * from "+username+" where FileName='"+dirName+"' and FileType=0 and P_Dir="+((TreeNode)node.getUserObject()).getP_FileKey();
						result=dbop.sqlOP(opStr);
						if(result.next()) {
							int fileID=result.getInt("FileKey");
							int fileType=result.getInt("FileType");
							int p=result.getInt("P_Dir");
							
							if(((TreeNode)node.getUserObject()).getFileType()==-1) {
								((TreeNode)node.getUserObject()).setNode(dirName,fileID,0,p);
								node.add(new DefaultMutableTreeNode(new TreeNode("<空目录>",-1,-1,p)));
							}
							else {
								DefaultMutableTreeNode newNode=new DefaultMutableTreeNode(new TreeNode(dirName,fileID,fileType,p));
								newNode.add(new DefaultMutableTreeNode(new TreeNode("<空目录>",-1,-1,fileID)));
								
								pNode.add(newNode);
							}
							dt.reload();
							return;
						}
					} catch (HeadlessException e1) {
						// TODO 自动生成的 catch 块
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO 自动生成的 catch 块
						e1.printStackTrace();
					}
				}
				
				if(((TreeNode)node.getUserObject()).getFileType()==0) {//文件夹
					
					try {
						opStr="select * from "+username+" where FileName='"+dirName+"' and FileType=0 and P_Dir="+((TreeNode)node.getUserObject()).getFileKey();
						result=dbop.sqlOP(opStr);
						if(result.next()) {
							JOptionPane.showMessageDialog(null,"该目录名已存在！",null,JOptionPane.ERROR_MESSAGE);
							return;
						}
					} catch (SQLException e1) {
						// TODO 自动生成的 catch 块
						e1.printStackTrace();
					}
					
					try {
						opStr="select * from "+username+" where P_Dir="+((TreeNode)node.getUserObject()).getFileKey();
						result=dbop.sqlOP(opStr);
						if(!result.next()) {
							node.remove(0);
						}
						
						opStr="insert into "+username+" set FileName='"+dirName+"',FileType=0,P_Dir="+((TreeNode)node.getUserObject()).getFileKey();
						result=dbop.sqlOP(opStr);
						opStr="select * from "+username+" where FileName='"+dirName+"' and FileType=0 and P_Dir="+((TreeNode)node.getUserObject()).getFileKey();
						result=dbop.sqlOP(opStr);
						if(result.next()) {
							int fileID=result.getInt("FileKey");
							int p=result.getInt("P_Dir");
							int fileType=result.getInt("FileType");


							DefaultMutableTreeNode newNode=new DefaultMutableTreeNode(new TreeNode(dirName,fileID,fileType,p));
							newNode.add(new DefaultMutableTreeNode(new TreeNode("<空目录>",-1,-1,fileID)));
							
							node.add(newNode);
							
							dt.reload();
							
							return;
						}
					} catch (HeadlessException e1) {
						// TODO 自动生成的 catch 块
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO 自动生成的 catch 块
						e1.printStackTrace();
					}
				}
				
			}
		});
		
		menuItem2.addActionListener(new java.awt.event.ActionListener() { //复制
			public void actionPerformed(ActionEvent e) {
				clipBoard.clear();
				if(paths.length>1) {
					clipBoard=new ArrayList<TreePath>(Arrays.asList(paths));
					System.out.println(clipBoard.size());
				}
				else {
					clipBoard.add(path);
					System.out.println(clipBoard.size());
				}
					
				copyFlag=1;
			}
		});
		
		menuItem3.addActionListener(new java.awt.event.ActionListener() { //粘贴
			public void actionPerformed(ActionEvent e) {
				int i;
				TreeNode nd=(TreeNode)node.getUserObject();
				
				if(nd.getFileType()==1||nd.getFileType()==-1) {
					for(i=0;i<clipBoard.size();++i) {
						Object obj=clipBoard.get(i).getLastPathComponent();
							
						pNode.add(treeCopy(((TreeNode)(((DefaultMutableTreeNode)obj).getUserObject())).getFileKey(),nd.getP_FileKey()));
						if(nd.getFileType()==-1) {
							pNode.remove(node);
						}
					}
					
				}
				else {
					for(i=0;i<clipBoard.size();++i) {
						Object obj=clipBoard.get(i).getLastPathComponent();
							
						node.add(treeCopy(((TreeNode)(((DefaultMutableTreeNode)obj).getUserObject())).getFileKey(),nd.getFileKey()));
					}
				}
				
				if(copyFlag==0) {
					treeDelete(nd.getFileKey());
					pNode.remove(node);
				}
			}
		});
		
		menuItem4.addActionListener(new java.awt.event.ActionListener() { //删除
			public void actionPerformed(ActionEvent e) {
				int i;
				
				if(paths==null) {
					treeDelete(((TreeNode)node.getUserObject()).getFileKey());
					pNode.remove(node);
					dt.reload();
					return ;
					
				}
				
				for(i=0;i<paths.length;i++) {
					treeDelete(((TreeNode)(((DefaultMutableTreeNode)(paths[i].getLastPathComponent())).getUserObject())).getFileKey());
					Object obj[]=paths[i].getPath();
					((DefaultMutableTreeNode)obj[obj.length-2]).remove((DefaultMutableTreeNode)obj[obj.length-1]);
				}
				dt.reload();
			}
		});
		
		/*menuItem5.addActionListener(new java.awt.event.ActionListener() { //全选
			public void actionPerformed(ActionEvent e) {
				
			}
		});*/
		
		menuItem6.addActionListener(new java.awt.event.ActionListener() { //剪切
			public void actionPerformed(ActionEvent e) {
				clipBoard.clear();
				if(paths!=null)
					clipBoard=new ArrayList<TreePath>(Arrays.asList(paths));
				else {
					clipBoard.add(path);
				}
					
				copyFlag=0;
			}
		});
		
		
		menu.add(menuItem1);
		menu.add(menuItem2);
		menu.add(menuItem3);
		menu.add(menuItem4);
		//menu.add(menuItem5);
		return menu;
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		// TODO 自动生成的方法存根
		
	}

	@Override
	public void mousePressed(MouseEvent e) {
		// TODO 自动生成的方法存根
	 	TreePath path = tree.getPathForLocation(e.getX(), e.getY()); // 关键是这个方法的使用
	 	TreePath []paths = tree.getSelectionPaths();
	 	if (path == null) {  //JTree上没有任何项被选中
	 		return;
	 	}
	 	

	 	tree.setSelectionPath(path);
	 			
	 	Object obj[]=path.getPath();

	 	selectFile=((DefaultMutableTreeNode)(path.getLastPathComponent()));
	 	selectPFile=(obj.length>1)?(DefaultMutableTreeNode)obj[obj.length-2]:null;
	 	
	 	JPopupMenu menu;
	 	
	 	if (e.getButton() == 3) {
	 		if(obj.length!=1) {
	 			menu=setRightMouseMenu((DefaultMutableTreeNode)obj[obj.length-1],(DefaultMutableTreeNode)obj[obj.length-2],path,paths);
	 		}
	 		else {
	 			menu=setRightMouseMenu((DefaultMutableTreeNode)obj[obj.length-1],null,path,paths);
	 		}
	 		menu.show(tree, e.getX(), e.getY());

	 	}
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		// TODO 自动生成的方法存根
		
	}

	@Override
	public void mouseEntered(MouseEvent e) {
		// TODO 自动生成的方法存根
		
	}

	@Override
	public void mouseExited(MouseEvent e) {
		// TODO 自动生成的方法存根
		
	}
	
	
	private DefaultMutableTreeNode treeCopy(int fk,int pfk) {
		
		if(fk==-1)
			return null;
		
		String opStr="";
		ResultSet result=null;
		try {
			opStr="select * from "+username+" where FileKey="+String.valueOf(fk);
			result=dbop.sqlOP(opStr);
			if(result.next()) {
				
				String fileName=result.getString("FileName");
				int fileType=result.getInt("FileType");
				
				opStr="insert into "+username+" set FileName='"+fileName+"',FileType="+String.valueOf(fileType)+",P_Dir="+String.valueOf(pfk);
				dbop.sqlOP(opStr);
				opStr="select * from "+username+" where FileName="+fileName+",FileType="+String.valueOf(fileType)+",P_Dir="+String.valueOf(pfk);
				result=dbop.sqlOP(opStr);
				
				if(result.next()) {
					int fileID=result.getInt("FileKey");
					
					TreeNode tn=new TreeNode(fileName, fileID, fileType, pfk);
					DefaultMutableTreeNode nd=new DefaultMutableTreeNode(tn);
					
					opStr="select * from "+username+" where P_Dir="+String.valueOf(fk);
					result=dbop.sqlOP(opStr);
					while(result.next()) {
						int FileKey=result.getInt("FileKey");
						DefaultMutableTreeNode n = treeCopy(FileKey,fileID);
						if(n!=null)
							nd.add(n);
					}
					return nd;
				}
			}
		} catch (SQLException e) {
			// TODO 自动生成的 catch 块
			e.printStackTrace();
		}
		return null;
		
		
	}
	
	private void treeDelete(int fk) {
		String opStr="";
		ResultSet result=null;
		try {
			opStr="delete from "+username+" where FileKey="+String.valueOf(fk);
			dbop.sqlOP(opStr);
			
			opStr="select * from "+username+" where P_Dir="+String.valueOf(fk);
			result=dbop.sqlOP(opStr);
			while(result.next()) {
				int fid=result.getInt("FileKey");
				treeDelete(fid);
			}
			
		}
		catch (SQLException e) {
			// TODO 自动生成的 catch 块
			e.printStackTrace();
		}
	}
	
	
}
