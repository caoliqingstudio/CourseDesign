package hospital;

import hospital.SqlUse;
import javafx.stage.Stage;
import hospital.Patient;

//import java.awt.*;
import java.awt.event.*;
import java.sql.SQLException;
import java.awt.Dimension;
import java.awt.FlowLayout;
import javax.swing.JPanel;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.JPasswordField;
import javax.swing.JButton;
import hospital.Doctor;

public class login {
	public login() {
		JFrame f = new JFrame("Login");
		JPanel x = new JPanel();
		JPanel y = new JPanel();
		JPanel z = new JPanel();
		
        f.setSize(400, 300);
        f.setLocation(200, 200);
        f.setLayout(new FlowLayout());
        x.setSize(400, 100);
        y.setSize(400, 100);
        JLabel lName = new JLabel("�˺ţ�");
        // �����
        JTextField tfName = new JTextField("");
        //tfName.setText("�������˺�");
        tfName.setPreferredSize(new Dimension(200, 40));

        JLabel lPassword = new JLabel("���룺");
        // ����� 
        JPasswordField  tfPassword = new JPasswordField ("");
        //tfPassword.setText("����������");
        tfPassword.setPreferredSize(new Dimension(200, 40));

        x.add(lName);
        x.add(tfName);
        y.add(lPassword);
        y.add(tfPassword);
        
        JButton buttonP = new JButton("���˵�½");
        JButton buttonH = new JButton("ҽ����½");
        
        buttonP.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
            	//���˵�½ �����˺ŵ�
            	String name =tfName.getText();
            	String password = tfPassword.getText();
            	SqlUse sql = new SqlUse();
            	try {
            		String id=sql.loginPa(name, password);
					if(id== null) {
						//��½ʧ��
						 JOptionPane.showMessageDialog(f, "�˺Ż��������");
						System.out.println("false"+id);
					}else {
						//��½�ɹ�
						Patient win = new Patient();
						//Stage stage = new Stage();
						System.out.println("ok"+id);
						f.dispose();
						win.show(id);
					}
				} catch (SQLException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (Exception e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
            }
        });
        
        buttonH.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
            	//ҽ����½ �����˺ŵ�
            	String name =tfName.getText();
            	String password = tfPassword.getText();
            	SqlUse sql = new SqlUse();
            	try {
            		String id=sql.loginDo(name, password);
					if(id== null) {
						//��½ʧ��
						 JOptionPane.showMessageDialog(f, "�˺Ż��������");
						System.out.println("false"+id);
					}else {
						//��½�ɹ�
						System.out.println("ok"+id);
						Doctor win = new Doctor(id);
						win.setId(id);
						//Stage stage = new Stage();
						f.dispose();
						//win.show();
					}
				} catch (SQLException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (Exception e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
            }
        });
        
        z.add(buttonP);
        z.add(buttonH);
        
        f.add(x);
        f.add(y);
        f.add(z);
        
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        f.setVisible(true);
        //tfPassword.grabFocus();
	}
}
