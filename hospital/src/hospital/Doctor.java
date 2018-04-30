package hospital;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.table.AbstractTableModel;

public class Doctor{
	JFrame f=null;
    JMenuBar menubar;   //菜单条
    JMenu menuFile; //菜单
    JMenuItem itemPan,itemOpen, itemSave;   //菜单项
    String id="id";
	JTable tableP=null,tableI=null;
	Table_Model_Pan modelP=null;
	Table_Model_Inc modelI=null;
	JScrollPane s_pan = null;
	
	
	JTextField endTex;
	JTextField startTex;
	JTextField showDate1;
	JTextField showDate2;
	
    public Doctor(String string) {
        init(string);
        id = string;
        
        showPatient();
        //f.add(menubar);

    }
    
    //useless
    public void setId(String inId) {
    	id = inId;
    	f.setTitle("Doctor "+id);
    }
    void init(String s) {
        menubar = new JMenuBar();
        menuFile= new JMenu("菜单");
        menuFile.setMnemonic('F');
        itemPan= new JMenuItem("病人列表(P)");
        itemOpen = new JMenuItem("收入列表(I)");
        itemSave = new JMenuItem("退出(Q)");

        //设置菜单项的键盘操作方式是Ctrl+I和Ctrl+P键
        KeyStroke Ctrl_cutKey = 
                KeyStroke.getKeyStroke(KeyEvent.VK_I, InputEvent.CTRL_MASK);
        itemOpen.setAccelerator(Ctrl_cutKey);
        Ctrl_cutKey = 
                KeyStroke.getKeyStroke(KeyEvent.VK_P, InputEvent.CTRL_MASK);
        itemPan.setAccelerator(Ctrl_cutKey);
        Ctrl_cutKey = 
                KeyStroke.getKeyStroke(KeyEvent.VK_Q, InputEvent.CTRL_MASK);
        itemSave.setAccelerator(Ctrl_cutKey);
        menuFile.add(itemPan);
        menuFile.add(itemOpen);
        //menuFile.addSeparator();
        menuFile.add(itemSave);
        menubar.add(menuFile);  //将菜单添加到菜单条上
        
        //
        itemPan.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
            	showPatient();
            }
        });
        
        itemOpen.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
            	showIncome();
            }
        });
        
        itemSave.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
            	System.exit(0);
            }
        });
    }

    void showPatient() {
    	if(f!=null)  	f.dispose();
    	f = new JFrame("Doctor "+id);
    	modelP = new Table_Model_Pan(20);
    	tableP = new JTable(modelP);
    	//modelI = new Table_Model_Inc(20);
        //tableI = new JTable(modelI);
        f.setSize(600, 500);
        f.setLocation(200, 200);
        f.setLayout(new FlowLayout());

        f.setJMenuBar(menubar);
        /************************************************/
       showPatientTable();
        
        //tableP = new JTable(modelP);
        System.out.println(modelP.getRowCount());
        s_pan = new JScrollPane(tableP);
        f.add(s_pan, BorderLayout.CENTER);
        
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.setVisible(true);
    }
    
    void showIncome() {
    	if(f!=null)  	f.dispose();
    	f = new JFrame("Doctor "+id);
    	modelI = new Table_Model_Inc();
    	tableI = new JTable(modelI);
    	//modelI = new Table_Model_Inc(20);
        //tableI = new JTable(modelI);
        f.setSize(600, 500);
        f.setLocation(200, 200);
        f.setLayout(new FlowLayout());

        f.setJMenuBar(menubar);
        
        JButton button=new JButton("查找");
        /************************************************/
        
        JPanel x = new JPanel();
        x.setSize(600, 50);
        
        Calendar now = Calendar.getInstance();
        
        DateChooser dateChooser1 = DateChooser.getInstance("yyyy-MM-dd");
        String dataString=now.get(Calendar.YEAR)+"-"+(now.get(Calendar.MONTH) + 1)+"-"+now.get(Calendar.DAY_OF_MONTH);
        showDate1 = new JTextField(dataString);
        JLabel startT= new JLabel("开始时间：");
        dateChooser1.register(showDate1);
        startTex=new JTextField("00:00");
        JLabel endT= new JLabel("结束时间：");
        DateChooser dateChooser2 = DateChooser.getInstance("yyyy-MM-dd");
        endTex=new JTextField(now.get(Calendar.HOUR_OF_DAY)+":"+now.get(Calendar.MINUTE));
        showDate2 = new JTextField(dataString);
        dateChooser2.register(showDate2);
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
            	 String sTime=showDate1.getText()+" "+startTex.getText();
                 String eTime=showDate2.getText()+" "+endTex.getText();
                 showIncomTable(sTime,eTime);
            }	
        });
        //tableP = new JTable(modelP);
        System.out.println(modelI.getRowCount());
        s_pan = new JScrollPane(tableI);
        x.add(startT);
        x.add(dateChooser1);
        x.add(startTex);
        x.add(endT);
        x.add(dateChooser2);
        x.add(endTex);
        x.add(button);
        String sTime=showDate1.getText()+" 00:00";
        String eTime=showDate2.getText()+" "+endTex.getText();
        showIncomTable(sTime,eTime);
        f.add(x);
        f.add(s_pan);
        
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.setVisible(true);
    }

    void showIncomTable(String sTime,String eTime) {
    	modelI.removeAll();
    	SqlUse sql=new SqlUse();
    	Vector<Vector<String>> infor = new Vector<Vector<String>>();
    	try {
			sql.selectIncome(sTime, eTime, infor);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	modelI.addAll(infor);
    	tableI.show(false);
    	tableI.show(true);
    	System.out.println("incom:    "+infor);
    	//modelI.addRow(ksname, ysbh, ysname, type, ghrc, income);
    }
    
    void showPatientTable() {
    	SqlUse sql=new SqlUse();
    	Vector<Vector<String>> infor = new Vector<Vector<String>>();
    	try {
			sql.selectPatient(id, infor);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	for(int i=0; i<infor.size();i++) {
    		Vector<String> vec = infor.get(i);
    		modelP.addRow(vec.get(0), vec.get(1), vec.get(3), vec.get(2));
    	}
    }
}

class Table_Model_Pan extends AbstractTableModel {
    private static final long serialVersionUID = -3094977414157589758L;

    private Vector content = null;

    private String[] title_name = { "挂号编号", "病人名称", "挂号日期时间","号种名称" };

    public Table_Model_Pan() {
        content = new Vector();
    }

    public Table_Model_Pan(int count) {
        content = new Vector(count);
    }


    /** 
     * 加入一行内容 
     */
    public void addRow(String id, String name, String time,String type) {
        Vector v = new Vector(4);
        v.add(0, id);
        v.add(1, name);
        v.add(2, time);
        v.add(3,type);
        content.add(v);
    }

    public void removeAll() {
        content.removeAllElements();
    }

    public String getColumnName(int col) {
        return title_name[col];
    }

    public int getColumnCount() {
        return title_name.length;
    }

    public int getRowCount() {
        return content.size();
    }

    public Object getValueAt(int row, int col) {
        return ((Vector) content.get(row)).get(col);
    }

    public Class getColumnClass(int col) {
        return getValueAt(0, col).getClass();
    }
}

class Table_Model_Inc extends AbstractTableModel {
    private static final long serialVersionUID = -3094977414157589758L;

    private Vector content = null;

    private String[] title_name = { "科室名称", "医生编号", "医生名称","号种名称","挂号人次","收入合计" };

    public Table_Model_Inc() {
        content = new Vector();
    }

    public Table_Model_Inc(int count) {
        content = new Vector(count);
    }

    public void addAll(Vector<Vector<String>> vec) {
    	for(int i=0;i<vec.size();i++) {
    		content.add(vec.get(i));
    	}
    }
    /** 
     * 加入一行内容 
     */
    public void addRow(String ksname, String ysbh, String ysname,String type,String ghrc,String income) {
        Vector v = new Vector();
        v.add(0, ksname);
        v.add(1, ysbh);
        v.add(2, ysname);
        v.add(3,type);
        v.add(4,ghrc);
        v.add(5,income);

        content.add(v);
    }

    public void removeAll() {
        content.removeAllElements();
    }

    public String getColumnName(int col) {
        return title_name[col];
    }

    public int getColumnCount() {
        return title_name.length;
    }

    public int getRowCount() {
        return content.size();
    }

    public Object getValueAt(int row, int col) {
        return ((Vector) content.get(row)).get(col);
    }

    public Class getColumnClass(int col) {
        return getValueAt(0, col).getClass();
    }
}

