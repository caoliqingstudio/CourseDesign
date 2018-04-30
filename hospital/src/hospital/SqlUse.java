package hospital;

import java.sql.*;
import java.util.Collection;
import java.util.Vector;

public class SqlUse {
    static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
    static final String DB_URL = "jdbc:mysql://localhost:3306/JavaP";
 
    // 数据库的用户名与密码，需要根据自己的设置
    static final String USER = "root";
    static final String PASS = ;;;;""; //此处需要修改密码
    
    Connection conn = null;
    Statement stmt = null;
    
	public SqlUse(){
        
     }
	
	 private void connectSQL() throws Exception {
	        try{
	            // 注册 JDBC 驱动
	            Class.forName("com.mysql.jdbc.Driver");
	        
	            // 打开链接
	            //System.out.println("连接数据库...");
	            conn = DriverManager.getConnection(DB_URL,USER,PASS);
	        
	            // 执行查询
	            //System.out.println(" 实例化Statement对象...");
	            stmt = conn.createStatement();
	            // 完成后关闭
	            //stmt.close();
	            //conn.close();
	        }catch(SQLException se){
	            // 处理 JDBC 错误
	            se.printStackTrace();
	            throw se;
	        }catch(Exception e){
	            // 处理 Class.forName 错误
	            e.printStackTrace();
	            throw e; 
	        }
	 }
	 
	 private void disconnectSQL() {
         /*try{
             if(stmt!=null) stmt.close();
         }catch(SQLException se2){
        	// 什么都不做
         }*/
         try{
             if(conn!=null) conn.close();
         }catch(SQLException se){
             se.printStackTrace();
         }
	 }
	 
	 public String loginPa(String name,String password) throws SQLException {
		 String state=null;
		 try{
			 connectSQL();
		 }catch(Exception e) {
			 System.out.println("fail to connect Mysql");
			 return state;
		 }
		 
         String sql;
         sql = "SELECT * FROM T_BRXX where BRMC = '"+name+"' AND  DLKL='"+password+"'";
         ResultSet rs = stmt.executeQuery(sql);

		 //System.out.println(sql); 
         if(rs.next()) {

    		 state= rs.getString("BRBH");
        	 rs = conn.createStatement().executeQuery("SELECT now()");
        	 if(!rs.next()) {
        		 return null;
        		 //System.out.println(sql);
        	 }
        	 //System.out.println(rs.getString("now()"));
        	 String t = rs.getString("now()");
        	 String upinfor ="UPDATE T_BRXX SET DLRQ='"+t+"' where BRBH= '"+state+"'";
        	 //System.out.println("upinfor"); 
        	 conn.createStatement().execute(upinfor);
        }
		 disconnectSQL();
		return state;
	 }
	 
	 public String loginDo(String name,String password) throws SQLException {
			 String state=null;
			 try{
				 connectSQL();
			 }catch(Exception e) {
				 System.out.println("fail to connect Mysql");
				 return state;
			 }
			 
	         String sql;
	         sql = "SELECT * FROM T_KSYS where YSMC = '"+name+"' AND  DLKL='"+password+"'";
	         ResultSet rs = stmt.executeQuery(sql);

			 //System.out.println(sql);
	         if(rs.next()) {
	        	 state= rs.getString("YSBH");
	        	 rs = conn.createStatement().executeQuery("SELECT now()");
	        	 if(!rs.next()) {
	        		 return null;
	        		 //System.out.println(sql);
	        	 }
	        	 //System.out.println(rs.getString("now()"));
	        	 String t = rs.getString("now()");
	        	 String upinfor ="UPDATE T_KSYS SET DLRQ='"+t+"' where YSBH= '"+state+"'";
	        	 //System.out.println("upinfor"); 
	        	 conn.createStatement().execute(upinfor);
	         }
			 disconnectSQL();
			return state;
		 }

	 public void selectPatient(String ysbh,Vector<Vector<String>> infor) throws SQLException {
		 try{
			 connectSQL();
		 }catch(Exception e) {
			 System.out.println("fail to connect Mysql");
			 return;
		 }
		 String sql;
         sql = "SELECT * FROM T_GHXX WHERE YSBH = '"+ysbh+"'";
         ResultSet rs = stmt.executeQuery(sql);
        while(rs.next()) {
        	if(rs.getString("THBZ").equals("0")) {
        		Vector<String> item = new Vector<String>(4);
           	 	item.add(0,  rs.getString("GHBH"));
           	 	item.add(1, getBRMC(rs.getString("BRBH")));
           	 	item.add(2, getHZLB( rs.getString("HZBH")));
           	 	item.add(3,  rs.getString("RQSJ"));
           	 	infor.add(item);
        	}
         }
		 disconnectSQL();
	 }
	 
	 String getKSMC(String ksbh) {
		 String sql = "SELECT * FROM T_KSXX WHERE KSBH ='"+ksbh+"'";
         ResultSet rs = null;
		String str=null;
         try {
			rs = conn.createStatement().executeQuery(sql);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        try {
			if(rs.next()) {
				str = rs.getString("KSMC");
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        return str;
	 }
	 
	 void getYSXX(Vector<Vector<String>> infor) {
		 String sql = "SELECT * FROM T_KSYS";
         ResultSet rs = null;
		try {
			rs = conn.createStatement().executeQuery(sql);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
         try {
			while(rs.next()) {
					Vector<String> item = new Vector<String>(6);
			   	 	item.add(0,  getKSMC(rs.getString("KSBH")));
			   	 	item.add(1, rs.getString("YSBH"));
			   	 	item.add(2, rs.getString("YSMC"));
			   	 	item.add(3,"普通号");
			   	 	item.add(4,"0");
			   	 	item.add(5,"0.00");
			   	 	infor.add(item);
			   	 	//System.out.println("sfzj   "+rs.getString("SFZJ"));
			   	 	if(rs.getString("SFZJ").equals("1")) {
			   	 		Vector<String> item2 = new Vector<String>(6);
			   	 		item2.add(0,  getKSMC(rs.getString("KSBH")));
			   	 		item2.add(1, rs.getString("YSBH"));
			   	 		item2.add(2, rs.getString("YSMC"));
			   	 		//item2.remove(3);
			   	 		item2.add(3,"专家号");
			   	 		item2.add(4,"0");
			   	 		item2.add(5,"0.00");
			   	 		infor.add(item2);
			   	 	}
			 }
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        //System.out.println(infor);
	 }
	 
	 public void selectIncome(String sTime,String eTime,Vector<Vector<String>> infor) throws SQLException {
		 try{
			 connectSQL();
		 }catch(Exception e) {
			 System.out.println("fail to connect Mysql");
			 return;
		 }
		 getYSXX(infor);
		 String sql;
		 
         sql = "SELECT * FROM T_GHXX WHERE RQSJ >= '"+sTime+"' and RQSJ <= '"+eTime+"'";
         ResultSet rs = stmt.executeQuery(sql);
        System.out.println(sql);
         while(rs.next()) {
        	 
        		String ysbh =rs.getString("YSBH");
        		String hzlb =getHZLB( rs.getString("HZBH"));
           	 	String ghfy=rs.getString("GHFY");
           	 	System.out.println(ysbh+hzlb+ghfy);
           	 	for(int i = 0;i<infor.size();i++) {
           	 		if(infor.get(i).get(1).equals(ysbh)) {
           	 			if(hzlb.equals("专家号")) {
           	 				i++;
           	 			}
           	 			String ghrc=infor.get(i).get(4);
           	 			String yssy=infor.get(i).get(5);
           	 			double yssyDouble = Double.parseDouble(yssy)+Double.parseDouble(ghfy);
           	 			int ghrcInt = Integer.parseInt(ghrc);
           	 			ghrcInt++;
           	 			Vector<String> temp = infor.get(i);
           	 			temp.remove(4);
           	 			temp.add(4, ""+ghrcInt);
           	 			temp.remove(5);
           	 			temp.add(5,""+yssyDouble);
           	 			infor.remove(i);
           	 			infor.add(i,temp);
           	 			break;
           	 		}
           	 	}
         }
		 disconnectSQL();
	 }
	 
	 
	 
	 public void selectKSXX(Vector infor) throws SQLException {
		 try{
			 connectSQL();
		 }catch(Exception e) {
			 System.out.println("fail to connect Mysql");
			 return;
		 }
		 String sql;
         sql = "SELECT * FROM T_KSXX";
         ResultSet rs = stmt.executeQuery(sql);
         while(rs.next()) {
        	 Vector<String> item = new Vector<String>(3);
        	 item.add(0,  rs.getString("KSBH"));
        	 item.add(1, rs.getString("KSMC"));
        	 item.add(2,  rs.getString("PYZS"));
        	 infor.add(item);
         }
		 disconnectSQL();
	 }
	 
	 public void selectYSXX(Vector infor,String ksbh) throws SQLException {
		 try{
			 connectSQL();
		 }catch(Exception e) {
			 System.out.println("fail to connect Mysql");
			 return;
		 }
		 String sql;
         sql = "SELECT * FROM T_KSYS WHERE KSBH = '"+ksbh+"'";
         ResultSet rs = stmt.executeQuery(sql);
         System.out.println(ksbh);
         while(rs.next()) {
        	 Vector<String> item = new Vector<String>(4);
        	 item.add(0,  rs.getString("YSBH"));
        	 item.add(1, rs.getString("YSMC"));
        	 item.add(2,  rs.getString("PYZS"));
        	 item.add(3,  rs.getString("SFZJ"));
        	 //System.out.println(item.get(3)+item.get(1)+item.get(2));
        	 infor.add(item);
         }
		 disconnectSQL();
	 }

	public void selectHZMC(Vector<Vector> hzmcInfor, String ksmcid, String hzlbid) throws SQLException {
		// TODO Auto-generated method stub
		 try{
			 connectSQL();
		 }catch(Exception e) {
			 System.out.println("fail to connect Mysql");
			 return;
		 }
		 String sql;
         sql = "SELECT * FROM T_HZXX WHERE KSBH = '"+ksmcid+"'";
         ResultSet rs = stmt.executeQuery(sql);
        while(rs.next()) {
        	if(rs.getString("SFZJ").equals(hzlbid)) {
        		Vector<String> item = new Vector<String>(5);
           	 	item.add(0,  rs.getString("HZBH"));
           	 	item.add(1, rs.getString("HZMC"));
           	 	item.add(2,  rs.getString("PYZS"));
           	 	item.add(3,  rs.getString("GHRS"));
           	 	item.add(4,  rs.getString("GHFY"));
           	 	System.out.println(item.get(4)+item.get(3)+item.get(2));
           	 	hzmcInfor.add(item);
        	}
         }
		 disconnectSQL();
	}
	
	public String selectBRYE(String brbh) throws SQLException {
		// TODO Auto-generated method stub
		String brye=null; 
		try{
			 connectSQL();
		 }catch(Exception e) {
			 System.out.println("fail to connect Mysql");
			 return null;
		 }
		 String sql;
        sql = "SELECT * FROM T_BRXX WHERE BRBH = '"+brbh+"'";
        ResultSet rs = stmt.executeQuery(sql);
       if(rs.next()) {
       	  	 brye=rs.getString("YCJE");
        }
		 disconnectSQL();
		 return brye;
	}
	
	public String getGHHM() throws SQLException {
		try{
			 connectSQL();
		 }catch(Exception e) {
			 System.out.println("fail to connect Mysql");
			 return null;
		 }
		 String sql;
       sql = "SELECT * FROM T_GHXX ORDER BY GHBH DESC LIMIT 1";
       ResultSet rs = stmt.executeQuery(sql);
      if(rs.next()) {
      	  	 return rs.getString("GHBH");
       }
		 disconnectSQL();
		return null;
	}
	
	// 1 成功 挂号编号有更改
	// 0 成功 
	//-1 因为费用不同失败
	//-2 因为挂号编号失败
	//-3 因为人次达到失败
	//-4 数据库连接失败
	//-5 其他原因
	public int pushGH(String ghbh,String hzbh,String ysbh,
			String brbh,String ghfy,String brye) throws SQLException {
		int state = 0;
		try{
			 connectSQL();
		 }catch(Exception e) {
			 System.out.println("fail to connect Mysql");
			 return -4;
		 }
		//检验编号
		String sql;
		 sql = "SELECT * FROM T_GHXX ORDER BY GHBH DESC LIMIT 1";
      	ResultSet rs = stmt.executeQuery(sql);
      	if(rs.next()) {
      		String sqlghbh=rs.getString("GHBH");
      		if(Integer.parseInt(sqlghbh)>=Integer.parseInt(ghbh)) {
      			int x=Integer.parseInt(sqlghbh);
      			ghbh=String.format("%6s", (x+1)).replaceAll(" ", "0");
      			state =1;
      		}
      	}
		//获取时间
      	rs = conn.createStatement().executeQuery("SELECT now()");
      	if(!rs.next()) {
      		return -5;
      		//System.out.println(sql);
      	}
      	//System.out.println(rs.getString("now()"));
      	String ghsj = rs.getString("now()");
      	
      	
      	//获取人次
      	sql ="SELECT * FROM T_GHXX WHERE HZBH='"+hzbh +"' ORDER BY GHBH DESC LIMIT 1";
      	rs = conn.createStatement().executeQuery(sql);
      	int ghrc;
      	if(rs.next()) {
      		ghrc= rs.getInt("GHRC");
      		ghrc++;
      	}else {
      		ghrc=0;
      	}
      	//获取上限及金额
      	sql = "SELECT * FROM T_HZXX WHERE HZBH = '"+hzbh+"'";
        rs = conn.createStatement().executeQuery(sql);
        if(rs.next()) {
       	  	 String newghfy=rs.getString("GHFY");
       	  	 if(!newghfy.equals(ghfy)) {
       	  		 state = 1;
       	  		 ghfy=newghfy;
       	  	 }
        }
      	
      	//插入
      	sql = "INSERT INTO T_GHXX VALUES('"+ghbh+"','"+hzbh+"','"+ysbh+"','"+brbh+"',"
      			+ghrc+", false,'"+ghfy+"','"+ghsj+"')";
      	System.out.println("gh : "+sql);
      	if(conn.createStatement().execute(sql)) {
      		return -5;
      	}
      	
      	//病人扣款
      	if(brye!=null) {
          	double x=Double.valueOf(brye.toString())-Double.valueOf(ghfy.toString());
            String upinfor ="UPDATE T_BRXX SET YCJE='"+x+"' where BRBH= '"+brbh+"'";
          	//System.out.println("upinfor"); 
          	conn.createStatement().execute(upinfor);
      	}
      	disconnectSQL();
		return state;
	}
	
	String getBRMC(String brbh) throws SQLException {
		String str=null;
		String sql="SELECT * FROM T_BRXX WHERE BRBH = '"+brbh+"'";
		ResultSet rss=conn.createStatement().executeQuery(sql);
		if(rss.next()) {
			str=rss.getString("BRMC");
		}
		return str;
	}
	String getHZLB(String hzbh) throws SQLException {
		String str=null;
		String sql="SELECT * FROM T_HZXX WHERE HZBH = '"+hzbh+"'";
		ResultSet rss=conn.createStatement().executeQuery(sql);
		if(rss.next()) {
			str=rss.getString("SFZJ").equals("1")?"专家号":"普通号";
		}
		return str;
	}

		 
}
