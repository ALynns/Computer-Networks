package panclient;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;

import tools.BrowseWake;

public class PanButtonListener implements ActionListener{
	
	private DefaultTreeModel dt;
	private DefaultMutableTreeNode root;
	private DefaultMutableTreeNode selectFile;
	private DefaultMutableTreeNode selectPFile;
	
	public PanButtonListener(DefaultTreeModel d,DefaultMutableTreeNode r,DefaultMutableTreeNode f,DefaultMutableTreeNode p) {
		dt=d;
		root=r;
		selectFile=f;
		selectPFile=p;
	}

	public void actionPerformed(ActionEvent panButtonEvent) {
		JFileChooser jfc=new JFileChooser();  
        jfc.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES );  
        jfc.showDialog(new JLabel(), "选择");  
        File file=jfc.getSelectedFile();  
        if(file.isDirectory()){  
            System.out.println("文件夹:"+file.getAbsolutePath());  
        }else if(file.isFile()){  
            System.out.println("文件:"+file.getAbsolutePath());  
        }  
        System.out.println(jfc.getSelectedFile().getName());  
	}
}
