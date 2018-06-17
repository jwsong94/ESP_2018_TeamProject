import java.awt.BorderLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.File;
import java.io.FileInputStream;
import java.io.DataOutputStream;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.filechooser.FileSystemView;



class SFrame extends JFrame implements ActionListener,Runnable{
	JButton Openbutton ,Lockbutton, but_input, Findbutton, defaultButton;
	JTextArea ta;
	JTextField tf; 
	
	static ServerSocket serverSocket = null;
	static Socket clientSocket =null;
	static PrintWriter out;
	static DataOutputStream dos;
	static boolean isReady = false;
	static BufferedReader in;
	static String inputLine, outputLine;	
	private BufferedInputStream bis;

	
	public SFrame(){
		setSize(500, 600);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("서버");
		
		//패널1
		JPanel panel = new JPanel();
		Openbutton = new JButton("Open");
		Openbutton.addActionListener(this);
		
		//파일 열기창
		JPanel panel4 = new JPanel();
		Findbutton = new JButton("Find");
		Findbutton.addActionListener(this);
		
		//디폴트
		JPanel panel3 = new JPanel();
		defaultButton = new JButton("Default");
		defaultButton.addActionListener(this);
		
		//패널2
		JPanel panel2 = new JPanel();
		ta = new JTextArea(30, 30);
		ta.setBounds(30, 0, 427, 400);
		tf = new JTextField(30);
		tf.setBounds(82, 430, 336, 24);
		but_input = new JButton("입력");
		but_input.setBounds(381, 477, 61, 27);
		but_input.addActionListener(this);
		
		Lockbutton = new JButton("Lock");
		Lockbutton.addActionListener(this);
		panel2.add(Lockbutton);
		panel2.add(Openbutton);
		panel2.add(defaultButton);
		panel2.add(Findbutton);
		
		panel.setLayout(null);
		panel.add(ta);
		panel.add(tf);
		panel.add(but_input);
		getContentPane().add(panel2, BorderLayout.NORTH);
		
		
		getContentPane().add(panel);
		setVisible(true);
		
	}

	@Override
	public void actionPerformed(ActionEvent arg0) {
		String s;
		s=tf.getText(); 
		
		//버튼 클릭해서 데이터 송신
		//out.println에 보내고 싶은 문자열을 보내면 클라이언트로 전송
		//Text Area에 문장을 적은 뒤 버튼을 클릭하면 클라이언트로 보내고 싶은 문자열 전송
		if(arg0.getSource()==but_input){
			ta.append(s);
			out.print(s);
			out.flush();
			tf.setText("");
		}else if(arg0.getSource()==Openbutton){
			System.out.println("Open");
			out.print(1 << 3);
			out.flush();
		}else if(arg0.getSource()==Lockbutton){
			System.out.println("Lock");
			out.print(1 << 2);
			out.flush();
		}else if(arg0.getSource()==defaultButton){
			System.out.println("Default");
			out.print(1 << 1);
			out.flush();
		}else if(arg0.getSource()==Findbutton) {
			jFileChooserUtil();
		}
	}
	
	public  void serverStart() throws IOException{
		System.out.println("서버 시작");
		
		try{
			serverSocket = new ServerSocket(11111);
		}catch(IOException e){
			System.out.println("다음의 포트 번호에 연결할 수 없습니다 : 11111");
			System.exit(1);
		}
		
		clientSocket = null;
		try{
			clientSocket = serverSocket.accept();
		}catch(IOException e){
			System.err.println("accept() 실패 ");
			System.exit(1);
		}
		
		out = new PrintWriter(clientSocket.getOutputStream(),true);
		dos = new DataOutputStream(clientSocket.getOutputStream());
		in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		
		outputLine = "server : hi";
		out.println(outputLine);
		ta.append("서버 메시지 : 클라이언트가 접속되었습니다.\n");
		bis = new BufferedInputStream(clientSocket.getInputStream());
		
		//읽는 부분
		while(true)
		{
			try
			{
				//원래 이 부분에서 동기화 값인 -2 를 확인하는 코드를 넣어놨었는데 현재 삭제하였습니다.
				byte[] buf = new byte[1024];
				bis.read(buf);
				String p;
				String str = new String(buf).trim();
				
				// "/" 구분자로 하여 읽은 데이터 분할
				String[] array = str.split("/");
				for(int i=0;i<array.length;i++) {
					System.out.println(array[i]);
				}
				
				if(array[0].equals("1")){
					System.out.println("people come");

					System.out.println("correct");
					p = array[1]+": aceept - " + array[2];
					ta.append(p);
					
				}if(array[0].equals("0")){
					System.out.println("wrong person");
					p = array[1]+": Not recognize";
					ta.append(p);
				}
				
					
				System.out.println(str);
				//ta.append(str+"\n");
			
				
			}
			catch(Exception e)
			{
				System.out.println("Server ClientThread Error");
				break;
			}
		}
		/*
		while((inputLine = in.readLine())!=null){
			String s =inputLine+"\n";
			System.out.println(s);
			ta.append(s);
			//outputLine = inputLine;
			//out.println(outputLine);
			if(outputLine.equals("quit"))
				break;
		}
		*/
	}
	
//탐색기에서 파일 열고 읽어서 클라이언트한테 출력하는 것을 구현중입니다.
//클라이언트와 동기화를 위해 -2 를 주고 받는데 이 과정에 문제가 있습니다.
//아마 C에서 보낸건 맞는데 자바에서 못 읽은거 같습니다. 이름전송까지는 완료되었습니다.
//그 후 다시 동기화 후 파일 전송에서 막혔습니다.
public static String jFileChooserUtil(){
		JPanel jpanel = new JPanel();
		JLabel image;
        String folderPath = "";
        FileFilter imageFilter = new FileNameExtensionFilter(
        		"Image files", ImageIO.getReaderFileSuffixes());
        File fileName;
        
        JFileChooser chooser = new JFileChooser(FileSystemView.getFileSystemView().getHomeDirectory()); // 디렉토리 설정
        chooser.setCurrentDirectory(new File("/")); // 현재 사용 디렉토리
        chooser.addChoosableFileFilter(imageFilter);
        chooser.setAcceptAllFileFilterUsed(false);   // Fileter 모든 파일 적용 
        chooser.setDialogTitle("Choose an image");
        chooser.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES); // 파일 선택 모드
      
        /*
        FileNameExtensionFilter filter = new FileNameExtensionFilter("Image Files", "cd11"); // filter 확장자 추가
        chooser.setFileFilter(filter); // 파일 필터를 추가
        */
        int returnVal = chooser.showOpenDialog(null); // 열기용 창 오픈
        
        if(returnVal == JFileChooser.APPROVE_OPTION) { // 열기를 클릭 
        	fileName=chooser.getSelectedFile();
        	  String sname = fileName.getAbsolutePath(); //THIS WAS THE PROBLEM
        	  File f = new File(sname);
        	 try { 
        		 FileInputStream imageFile = new FileInputStream(f);
        		 String name = f.getName();
        		 String[] splitStr;
        		 int readLen = 0;
        		 byte[] buffer = new byte[1024];
        		 
        		 System.out.println(sname);        		 
        		 System.out.println(name);
        		 //클라이언트가 받을 때가지 대기
        		 out.print(1);
        		 out.flush();
       
        		 splitStr = name.split(".");
        		 System.out.print(splitStr.length);
        		 
        		
        			 
        		 isReady = false;
        		 
        		 out.flush();
        		 
        		 while (isReady == false)
        			 System.out.println("asdf");
        		 isReady = false;
        		 
        		 while((readLen = imageFile.read(buffer)) != -1) {
                     dos.write(buffer, 0, readLen);
        		 }    		 
        		 dos.flush();
        		 imageFile.close();
        		 System.out.println("finished");        	 }
        	 catch (IOException e) {
        		 System.out.println(e);
        	 }
        	 
        }
        else if(returnVal == JFileChooser.CANCEL_OPTION){ // 취소를 클릭
            System.out.println("cancel"); 
            folderPath = "";
        }
        
        return folderPath;
        
    }

	@Override
	public void run() {
		SFrame f = new SFrame();
		try {
			f.serverStart();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}


