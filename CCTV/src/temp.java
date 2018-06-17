import java.awt.BorderLayout;
import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;

import com.sun.jna.NativeLibrary;

import uk.co.caprica.vlcj.component.EmbeddedMediaPlayerComponent;
import uk.co.caprica.vlcj.runtime.RuntimeUtil;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import javax.swing.JTextField;


public class temp extends JFrame{

	private JPanel contentPane;
	private static EmbeddedMediaPlayerComponent mediaplayer;
	private final String vlcpath = "C:/VLC";
	//vlc�÷��̾� ��ġ �� ��� ����

	public static int PORT = 11111;
	//private static String mediapath = "https://youtu.be/A9K-7AVG2i4";
	private static String mediapath = "http://192.168.0.39:8081/";
	//vlc �÷��̾� �׽�Ʈ �Ϸ��� mediapath �ּҸ� �����ؼ� �׽�Ʈ
	
	
	static ServerSocket serverSocket = null;
	static Socket clientSocket =null; 
	static PrintWriter out; 
	static BufferedReader in; 
	static String inputLine, outputLine;

	
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					
					temp frame = new temp();
					frame.setVisible(true);
									
					//frame.set();
					
					//��Ʃ�� ������ �÷���
					mediaplayer.getMediaPlayer().prepareMedia(mediapath);
					mediaplayer.getMediaPlayer().setPlaySubItems(true);
					mediaplayer.getMediaPlayer().play();
					
					Runnable r = new SFrame();
					Thread t2 = new Thread(r);
					t2.start();
					//t2.start();
				
					//frame.serverStart();
				
					//�̰Ŵ� ����� ������ ��û 
					//mediaplayer.getMediaPlayer().playMedia(mediapath);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
		
	
	/**
	 * Create the frame.
	 */
	public temp() {
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 1068, 689);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(null);
		
		NativeLibrary.addSearchPath(RuntimeUtil.getLibVlcLibraryName(), vlcpath);
		mediaplayer = new EmbeddedMediaPlayerComponent();
		
		JPanel panel = new JPanel();
		panel.setBounds(0, 26, 1036, 589);
		panel.setLayout(new BorderLayout());
		
		//�ؿ��� ����� �׽�Ʈ
        panel.add(mediaplayer, BorderLayout.CENTER);
        panel.setVisible(true);
        
		contentPane.add(panel);
		
		
	}
	
}
