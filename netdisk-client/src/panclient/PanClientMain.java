package panclient;

import java.sql.*;
import databaseconnect.*;
 

public class PanClientMain {
	
	static String sql = null;  
    static MariadbOP db1 = null;  
    static ResultSet ret = null;  
	
	public static void main(String[] args) {  
        Login login=new Login();
        login.showUI();
	}

}
