package hospital;

import hospital.Patient;

import java.awt.Component;
import java.net.URL;
import java.sql.SQLException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Collection;
import java.util.Date;
import java.util.ResourceBundle;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.event.DocumentEvent.EventType;

import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TextField;


public class PatientControl implements Initializable{

	@FXML
    private Button myButton1;
	@FXML
    private Button myButton2;
	@FXML
    private Button myButton3;


	@FXML
	private ComboBox<String> ksmc;
	@FXML
	private ComboBox<String> ysmc;
	@FXML
	private ComboBox<String> hzlb;
	@FXML
	private ComboBox<String> hzmc;
	@FXML
	private TextField jkje;
	@FXML
	private TextField yjje;
	@FXML
	private TextField zlje;
	@FXML
	private TextField ghhm;
	
	String ksmcid,ysmcid,hzlbid,hzmcid,jkjeid,yjjeid,ghbhid;
	String ycje,brbh;
	
	Vector<Vector> ksmcInfor,ysmcInfor,hzmcInfor;
	Vector<String> hzlbInfor;
	
	@Override
	public void initialize(URL location, ResourceBundle resources) {
		// TODO Auto-generated method stub
		brbh=Patient.id;
		ksmcInfor = new Vector();
		ysmcInfor= new Vector();
		hzlbInfor= new Vector();
		hzmcInfor = new Vector();
		SqlUse sql = new SqlUse();
		try {
			sql.selectKSXX(ksmcInfor);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		for(int i=0;i<ksmcInfor.size();i++) {
			String itemString = (String) ksmcInfor.get(i).get(0)+" "+(String) ksmcInfor.get(i).get(1)+" "+(String) ksmcInfor.get(i).get(2);
			ksmc.getItems().add(itemString);
		}
		try {
			ycje=sql.selectBRYE(brbh);
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		ksmc.getSelectionModel().selectedItemProperty().addListener(new ChangeListener() {  
            @Override  
            public void changed(ObservableValue observable, Object oldValue, Object newValue) {  
                System.out.println("成功选中"+newValue.toString());
                ksmcid=newValue.toString().substring(0, 6);
                SqlUse sql = new SqlUse();
                ysmcInfor.clear();
        		try {
        			sql.selectYSXX(ysmcInfor, ksmcid);
        		} catch (SQLException e) {
        			// TODO Auto-generated catch block
        			e.printStackTrace();
        		}
        		ysmc.getEditor().setText("x");
        		ysmc.getEditor().setText("");
            }  
        });  

		ysmc.getSelectionModel().selectedItemProperty().addListener(new ChangeListener() {  
            @Override  
            public void changed(ObservableValue observable, Object oldValue, Object newValue) {  
                //System.out.println("成功选中"+newValue.toString());
                ysmcid=newValue.toString().substring(0, 6);
                hzlbInfor.clear();
                String hzlbStr="普通号  false 0";
                hzlbInfor.add(hzlbStr);
                for(int i=0;i<ysmcInfor.size();i++) {
                	if(ysmcInfor.get(i).get(0).equals(ysmcid)) {
                		if(ysmcInfor.get(i).get(3).toString().equals("1")) {
                            String hzlbString="专家号 true 1";
                            hzlbInfor.add(hzlbString);
                		}
                		break;
                	}
                }
        		hzlb.getEditor().setText("x");
        		hzlb.getEditor().setText("");
            }  
        });  
		 
		hzlb.getSelectionModel().selectedItemProperty().addListener(new ChangeListener() {  
            @Override  
            public void changed(ObservableValue observable, Object oldValue, Object newValue) {  
                System.out.println("成功选中"+newValue.toString());
                hzlbid=newValue.equals("专家号 true 1")?"1":"0";
                SqlUse sql = new SqlUse();
                ysmcInfor.clear();
        		try {
        			sql.selectHZMC(hzmcInfor, ksmcid,hzlbid);
        		} catch (SQLException e) {
        			// TODO Auto-generated catch block
        			e.printStackTrace();
        		}
        		hzmc.getEditor().setText("x");
        		hzmc.getEditor().setText("");
            }  
        }); 
		
		hzmc.getSelectionModel().selectedItemProperty().addListener(new ChangeListener() {  
            @Override  
            public void changed(ObservableValue observable, Object oldValue, Object newValue) {  
                System.out.println("hzmc:"+newValue.toString());
                hzmcid=newValue.toString().substring(0, 6);
                for(int i=0;i<hzmcInfor.size();i++) {
                	if(hzmcInfor.get(i).get(0).equals(hzmcid)) {
                		yjjeid=(String) hzmcInfor.get(i).get(4);
                		yjje.setText(yjjeid);
                		break;
                	}
                }
                if(Double.valueOf(yjjeid.toString())<Double.valueOf(ycje.toString())) {
                	jkje.setEditable(false);
                	zlje.setEditable(false);
                	
                }else {
                	jkje.setEditable(true);
                	zlje.setEditable(true);                	
                }
                SqlUse sql = new SqlUse();
                try {
					ghbhid = sql.getGHHM();
					if(ghbhid==null) {
						ghbhid = "000000";
					}else {
						int ghbhidInt = Integer.parseInt(ghbhid);
						ghbhid=String.format("%6s", ghbhidInt+1).replaceAll(" ", "0");
					}
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
                ghhm.setText(ghbhid);
            }  
        }); 
		jkje.textProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                double x=Double.parseDouble(newValue)-Double.parseDouble(yjjeid);
                zlje.setText(""+x);
            }
        });

		 ksmc.setEditable(true);
		 ysmc.setEditable(true);
		 hzmc.setEditable(true);
		 hzlb.setEditable(true);
		 editChange1(ksmcInfor,ksmc);
		 editChange2(ysmcInfor,ysmc);
		 editChange3(hzlbInfor,hzlb);
		 editChange4(hzmcInfor,hzmc);
	}
	
	public void confirm(ActionEvent event) {
		//System.out.println("Button Clicked!"+Patient.id);
		SqlUse sql=new SqlUse();
		int state=0;
		try {
			String inycje=null;
			if(!jkje.isEditable()) {
				inycje=ycje;
				//System.out.println("208        ");
			}
			state =sql.pushGH(ghbhid, hzmcid, ysmcid, brbh,yjjeid,inycje);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Alert information = new Alert(Alert.AlertType.INFORMATION,"Welocme to JavaFX");
		information.setTitle("挂号提示"); //设置标题，不设置默认标题为本地语言的information
		information.setHeaderText("挂号状态"); //设置头标题，默认标题为本地语言的information
		switch(state) {
		case 0:
			information.setContentText("挂号成功");
			cancle(event);
			break;
		case 1:
//			JOptionPane.showMessageDialog(new JFrame().getContentPane(), "挂号成功，但费用更改");
			information.setContentText("挂号成功，但费用更改");
				cancle(event);
			break;
		case -3:
			information.setContentText("挂号失败，今日无号");
			//JOptionPane.showMessageDialog(new JFrame().getContentPane(), "挂号失败，今日无号");
			break;
		default:
			information.setContentText("挂号失败");
			//JOptionPane.showMessageDialog(new JFrame().getContentPane(), "挂号失败！");
		}
		information.showAndWait(); //显示弹窗，同时后续代码等挂起
		
	}
	
	public void cancle(ActionEvent event) {
		System.out.println("Button Clicked!");
		ysmc.getItems().clear();
		hzmc.getItems().clear();
		hzlb.getItems().clear();
		ysmcInfor.clear();
		hzmcInfor.clear();
		hzlbInfor.clear();
		jkje.setText("");
		yjje.setText("");
		zlje.setText("");
		ghhm.setText("");
	}
	
	public void quitWindow(ActionEvent event) {
		//System.out.println("Button Clicked!");
		System.exit(0);
		/*try {
			(((Application) event.getSource())).stop();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/
	}
	
	public void editChange1(Vector<Vector> vec,ComboBox com) {
		//com.setMouseTransparent(true);
		com.getEditor().textProperty().addListener(new ChangeListener<String>() {

		    @Override
		    public void changed(ObservableValue<? extends String> observable, 
		                                    String oldValue, String newValue) {
		    	if(newValue.length()>10) {
		    		return;
		    	}
		    	com.getItems().clear();
		        if(newValue.length()==0) {
		        	for(int i=0;i<vec.size();i++) {
		    			String itemString = (String) vec.get(i).get(0)+" "+(String) vec.get(i).get(1)+" "+(String) vec.get(i).get(2);
		    			com.getItems().add(itemString);
		    		}
		        }else {
		        	for(int i=0;i<vec.size();i++) {
		        		if(((String)vec.get(i).get(1)).contains(newValue)||((String)vec.get(i).get(2)).contains(newValue)){
		    				String itemString = (String) vec.get(i).get(0)+" "+(String) vec.get(i).get(1)+" "+(String) vec.get(i).get(2);
		    				com.getItems().add(itemString);
						}
		    		}
		        }
		        if(com.getItems().size()==1) {
		        	 com.getSelectionModel().select(0);
		        }
		        
		    }
		});
	}
	
	public void editChange2(Vector<Vector> vec,ComboBox com) {
		//com.setMouseTransparent(true);
		com.getEditor().textProperty().addListener(new ChangeListener<String>() {

		    @Override
		    public void changed(ObservableValue<? extends String> observable, 
		                                    String oldValue, String newValue) {
		    	if(newValue.length()>10) {
		    		return;
		    	}
		    	com.getItems().clear();
		        if(newValue.length()==0) {
		        	for(int i=0;i<vec.size();i++) {
		    			String itemString = (String) vec.get(i).get(0)+" "+(String) vec.get(i).get(1)+" "+(String) vec.get(i).get(2);
		    			com.getItems().add(itemString);
		    		}
		        }else {
		        	for(int i=0;i<vec.size();i++) {
		        		if(((String)vec.get(i).get(1)).contains(newValue)||((String)vec.get(i).get(2)).contains(newValue)){
		    				String itemString = (String) vec.get(i).get(0)+" "+(String) vec.get(i).get(1)+" "+(String) vec.get(i).get(2);
		    				com.getItems().add(itemString);
						}
		    		}
		        }
		        if(com.getItems().size()==1) {
		        	 com.getSelectionModel().select(0);
		        }
		    }
		});
	}
	
	public void editChange3(Vector<String> vec,ComboBox com) {
		//com.setMouseTransparent(true);
		com.getEditor().textProperty().addListener(new ChangeListener<String>() {

		    @Override
		    public void changed(ObservableValue<? extends String> observable, 
		                                    String oldValue, String newValue) {
		    	if(newValue.length()>5) {
		    		return;
		    	}
		    	com.getItems().clear();
		        if(newValue.length()==0) {
		        	for(int i=0;i<vec.size();i++) {
		    			String itemString =vec.get(i);
		    			com.getItems().add(itemString);
		    		}
		        }else {
		        	for(int i=0;i<vec.size();i++) {
		        		if(((String)vec.get(i)).contains(newValue)){
		    				String itemString = (String) vec.get(i);
		    				com.getItems().add(itemString);
						}
		    		}
		        }
		        if(com.getItems().size()==1) {
		        	 com.getSelectionModel().select(0);
		        }
		    }
		});
	}

	public void editChange4(Vector<Vector> vec,ComboBox com) {
		//com.setMouseTransparent(true);
		com.getEditor().textProperty().addListener(new ChangeListener<String>() {

		    @Override
		    public void changed(ObservableValue<? extends String> observable, 
		                                    String oldValue, String newValue) {
		    	if(newValue.length()>10) {
		    		return;
		    	}
		    	com.getItems().clear();
		        if(newValue.length()==0) {
		        	for(int i=0;i<vec.size();i++) {
		    			String itemString = (String) vec.get(i).get(0)+" "+(String) vec.get(i).get(1)+" "+(String) vec.get(i).get(2);
		    			com.getItems().add(itemString);
		    		}
		        }else {
		        	for(int i=0;i<vec.size();i++) {
		        		if(((String)vec.get(i).get(1)).contains(newValue)||((String)vec.get(i).get(2)).contains(newValue)){
		    				String itemString = (String) vec.get(i).get(0)+" "+(String) vec.get(i).get(1)+" "+(String) vec.get(i).get(2);
		    				com.getItems().add(itemString);
						}
		    		}
		        }
		        if(com.getItems().size()==1) {
		        	 com.getSelectionModel().select(0);
		        }
		    }
		});
	}
}
