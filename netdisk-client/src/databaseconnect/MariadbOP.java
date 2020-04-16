package databaseconnect;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.sql.*;

import panclient.TreeNode;


public class MariadbOP {
	public static final String url = "jdbc:mariadb://116.62.122.61/cloud";  
    public static final String name = "org.mariadb.jdbc.Driver";  
    public static final String user = "root";  
    public static final String password = "Wsygcr123";  
  
    public Connection conn = null;  
    public Statement statement=null; 
    public ResultSet lastResult=null;
    public MariadbOP() {  
        try {  
            Class.forName(name);//指定连接类型  
            conn = DriverManager.getConnection(url,user,password);//获取连接 
            statement=conn.createStatement();
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }
    
    public ResultSet sqlOP(String sql) {
    	try {
    		lastResult=statement.executeQuery(sql);
		} catch (SQLException e) {
			// TODO 自动生成的 catch 块
			e.printStackTrace();
		} 
    	return lastResult;
    }
  
    public void close() {  
        try {  
            this.conn.close();  
            this.statement.close();  
        } catch (SQLException e) {  
            e.printStackTrace();  
        }  
    } 
	
	
}
