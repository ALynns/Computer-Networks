package panclient;

public class TreeNode {
	private String fileName;
	private int fileID;
	private int fileType;
	private int p_FileKey;
	
	public TreeNode(String fn) {
		fileName=fn;
	}
	
	public TreeNode(String fn,int id) {
		fileName=fn;
		fileID=id;
	}
	
	public TreeNode(String fn,int id,int tp) {
		fileName=fn;
		fileID=id;
		fileType=tp;
	}
	
	public TreeNode(String fn,int id,int tp,int pid) {
		fileName=fn;
		fileID=id;
		fileType=tp;
		p_FileKey=pid;
	}
	
	public int getFileKey() {
		return fileID;
	}
	
	public int getFileType() {
		return fileType;
	}
	
	public int getP_FileKey() {
		return p_FileKey;
	}
	
	public void setNode(String fn) {
		fileName=fn;
	}
	
	public void setNode(String fn,int id) {
		fileName=fn;
		fileID=id;
	}
	
	public void setNode(String fn,int id,int tp) {
		fileName=fn;
		if(id>=0)
			fileID=id;
		fileType=tp;
	}
	
	public void setNode(String fn,int id,int tp,int pid) {
		fileName=fn;
		fileID=id;
		fileType=tp;
		p_FileKey=pid;
	}
	
	
	public String toString() {
		return fileName;
	}
}
