import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;

class SFrame extends JFrame implements ActionListener,Runnable{
	JButton Openbutton, Lockbutton, defaultButton, but_input;
	JTextArea ta;
	JTextField tf; 
	
	static ServerSocket serverSocket = null;
	static Socket clientSocket =null;
	static PrintWriter out;
	static BufferedReader in;
	static String inputLine, outputLine;	
	private BufferedInputStream bis;
	
	public SFrame(){
		setSize(500, 600);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("����");
		
		//�г�1
		JPanel panel = new JPanel();
		Openbutton = new JButton("Open");
		Openbutton.addActionListener(this);
		//�г�2
		JPanel panel2 = new JPanel();
		ta = new JTextArea(30, 30);
		ta.setBounds(30, 0, 427, 400);
		tf = new JTextField(30);
		tf.setBounds(82, 430, 336, 24);
		but_input = new JButton("�Է�");
		but_input.setBounds(381, 477, 61, 27);
		but_input.addActionListener(this);
		
		Lockbutton = new JButton("Lock");
		Lockbutton.addActionListener(this);
		panel2.add(Lockbutton);
		
		panel2.add(Openbutton);
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
		
		//��ư Ŭ���ؼ� ������ �۽�
		//out.println�� ������ ���� ���ڿ��� ������ Ŭ���̾�Ʈ�� ����
		//Text Area�� ������ ���� �� ��ư�� Ŭ���ϸ� Ŭ���̾�Ʈ�� ������ ���� ���ڿ� ����
		if(arg0.getSource()==but_input){
			ta.append(s+"\n");
			out.println(s);
			tf.setText("");
		}else if(arg0.getSource()==Openbutton){
			System.out.println("Open");
			out.println("open");
		}else if(arg0.getSource()==Lockbutton){
			System.out.println("Lock");
			out.println("lock");
		}
	}
	
	public  void serverStart() throws IOException{
		System.out.println("���� ����");
		
		try{
			serverSocket = new ServerSocket(11111);
		}catch(IOException e){
			System.out.println("������ ��Ʈ ��ȣ�� ������ �� �����ϴ� : 11111");
			System.exit(1);
		}
		
		clientSocket = null;
		try{
			clientSocket = serverSocket.accept();
		}catch(IOException e){
			System.err.println("accept() ���� ");
			System.exit(1);
		}
		
		out = new PrintWriter(clientSocket.getOutputStream(),true);
		in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		
		outputLine = "server : hi";
		out.println(outputLine);
		ta.append("���� �޽��� : Ŭ���̾�Ʈ�� ���ӵǾ����ϴ�.\n");
		bis = new BufferedInputStream(clientSocket.getInputStream());
		
		//�д� �κ�
		while(true)
		{
			try
			{
				byte[] buf = new byte[1024];
				bis.read(buf);
				String p;
				String str = new String(buf).trim();
				
				// "/" �����ڷ� �Ͽ� ���� ������ ����
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


