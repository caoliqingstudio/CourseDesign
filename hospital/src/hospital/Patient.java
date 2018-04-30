package hospital;

import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

import javafx.application.*;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class Patient extends Application{
	
	public static String id="000003";
	private Stage tempStage=null;

	public Patient() {
		//id="111";
		System.out.println("wrong");
	}
	
	@Override
	public void start(Stage primaryStage){
		System.out.println("start  s s "+this.id);
		// TODO Auto-generated method stub
	    this.tempStage = primaryStage;
		try {
            // Read file fxml and draw interface.
            Parent root = FXMLLoader.load(getClass().getResource("PatientUse.fxml"));
            primaryStage.setTitle("Patient "+id);
            primaryStage.setScene(new Scene(root));
            primaryStage.show();
            
        } catch(Exception e) {
            e.printStackTrace();
        }
	}
	
	public void show(String inid) {
		id =inid;
		System.out.println(" "+id);
		if(tempStage!=null)tempStage.setTitle("Patient "+id);
		extracted();
		//System.out.println(ksmc);
		//ksmc = new TextField();
		/*ksmc.textProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                System.out.println("状态：当前字符数为：");
            }
        });*/
		/*
		ksmc.focusedProperty().addListener(new ChangeListener<Boolean>()
		{
		    @Override
		    public void changed(ObservableValue<? extends Boolean> arg0, Boolean oldPropertyValue, Boolean newPropertyValue)
		    {
		        if (newPropertyValue)
		        {
		            System.out.println("Textfield on focus");
		        }
		        else
		        {
		            System.out.println("Textfield out focus");
		        }
		    }
		});*/
	}

	private void extracted() {
		launch(null);
	}
}
