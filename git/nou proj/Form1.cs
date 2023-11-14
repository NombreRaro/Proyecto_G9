using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.Security.Cryptography;

namespace ejercicio_individual
{
    public partial class Form1 : Form
    {
        Socket server;
        Thread atender;
        int codigo=0,conectado=0;
        string username,password, mssg, altura;
        Form2 f2 = new Form2();
        public Form1()
        {
            
            InitializeComponent();
            button1.BackColor = Color.Red;
            button6.BackColor = Color.Red;
            BackColor = Color.Red;
            label1.Visible = false;
            label3.Visible = false;
            textBox1.Visible = false;
            textBox2.Visible = false;
            button2.Visible = false;
            button5.Visible = false;
            radioButton1.Visible = false;
            radioButton2.Visible = false;
            radioButton3.Visible = false;
            button4.Visible = false;
            dataGridView1.Visible = false;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            this.codigo = 1;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            this.codigo = 2;
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            this.codigo = 3;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            try
            {
                this.mssg = this.codigo+"/hola";
                byte[] m = System.Text.Encoding.ASCII.GetBytes(mssg);
                server.Send(m);
                //Rebem la resposta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                if (this.codigo == 1)
                {
                    mssg = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    string[] array = mssg.Split(',');
                    MessageBox.Show("El jugador con mas partidas ganadas es: " + array[0] + ",con " + array[1] + " victorias");
                }
                else if (this.codigo == 2)
                {
                    mssg = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    string[] array = mssg.Split(',');
                    MessageBox.Show("El jugador mas malo es: " + array[0] + ",con " + array[1] + " victorias");
                }
                else if (this.codigo == 3)
                {
                    mssg = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    MessageBox.Show("El jugador mas guapo es: " + this.mssg + " felicidadades");
                }
               
                   
                
               
            }
            catch (SocketException ex)
            {
                Console.WriteLine(ex);
                return;
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            this.codigo = 4;
            this.mssg = this.codigo + "/hola";
            byte[] m = System.Text.Encoding.ASCII.GetBytes(mssg);
            server.Send(m);
            //Rebem la resposta del servidor
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            
            mssg = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            MessageBox.Show("Jugadores conectados: " + this.mssg);
            string[] array = mssg.Split('/');
            int num = Convert.ToInt16(array[0]);
            dataGridView1.ColumnCount = 1;
            dataGridView1.RowCount = num;
            dataGridView1.Columns[0].Name = "Username";
            for(int i = 1; i <= num; i++)
            {
                dataGridView1.Rows.Add(array[i]);
            }
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void button6_Click(object sender, EventArgs e)
        {
            if(button6.BackColor == Color.Red)
            {
                MessageBox.Show("Primero tiene que establecer conexion con el servidor...");
            }
            else
            {
                this.username = "";
                this.password = "";
                f2.ShowDialog();
                string creds = f2.getCreds();
                string[] array = creds.Split(':');
                this.username = array[0];
                this.password = array[1];
                try
                {
                    this.mssg = "6/" + this.username + "/" + this.password;
                    byte[] m = System.Text.Encoding.ASCII.GetBytes(mssg);
                    server.Send(m);
                    //Rebem la resposta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mssg = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    MessageBox.Show(this.mssg);
                    if (this.mssg == "1")
                    {
                        BackColor = Color.Green;
                        MessageBox.Show("Usted se ha registrado");
                        this.conectado = 1;
                    }
                    else
                    {
                        MessageBox.Show("Usted ha introducido algun campo incorrecto");
                        button6_Click(sender, e);   
                    }
                }
                catch (SocketException ex)
                {
                    Console.WriteLine(ex);
                    return;
                }
            }
            
        }

        private void tableLayoutPanel23_Paint(object sender, PaintEventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            IPAddress newaddress = IPAddress.Parse("192.168.1.38");
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint remoteEP = new IPEndPoint(newaddress, 9053);
            try
            {
                server.Connect(remoteEP);
                MessageBox.Show("Conectado");
            }
            catch (SocketException ex)
            {
                Console.WriteLine("No se ha podido conectar al servidor");
                Console.WriteLine(ex);
                return;
            }
            button1.BackColor = Color.Green;
            button6.BackColor = Color.Green;
            label1.Visible = true;
            label3.Visible = true;
            textBox1.Visible = true;
            textBox2.Visible = true;
            button2.Visible = true;
            button5.Visible = true;
            radioButton1.Visible = true;
            radioButton2.Visible = true;
            radioButton3.Visible = true;
            button4.Visible = true;
            dataGridView1.Visible = true;
        }



        private void button3_Click(object sender, EventArgs e)
        {
            this.codigo = 10;
            try
            {
                button1.BackColor = Color.Red;
                BackColor = Color.Red;
                this.mssg = this.codigo + "/";
                
                byte[] m = System.Text.Encoding.ASCII.GetBytes(mssg);
                server.Send(m);
                server.Shutdown(SocketShutdown.Both);
                server.Close();
            }
            catch (SocketException ex)
            {
                Console.WriteLine("No se ha podido desconectar del servidor");
                Console.WriteLine(ex);
                return;
            }

        }
        private void button2_Click(object sender, EventArgs e)
        {
            
                this.username = textBox1.Text;
                this.password = textBox2.Text;
                if (this.username == "" || this.password == "")
                {
                    MessageBox.Show("[!]FALTA INTRODUCIR CREDENCIALES");
                }
                else
                {
                    try
                    {
                        this.mssg = "0/" + this.username + "/" + this.password;
                        byte[] m = System.Text.Encoding.ASCII.GetBytes(mssg);
                        server.Send(m);
                        //Rebem la resposta del servidor
                        byte[] msg2 = new byte[80];
                        server.Receive(msg2);
                        mssg = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                        if (this.mssg == "1")
                        {
                            BackColor = Color.Green;
                            MessageBox.Show("Usted se ha loggeado");
                            this.conectado = 1;
                        }
                        else
                        {
                            MessageBox.Show("Usted no ha introducido unas credenciales correctas. Debe registrarse:");
                        }
                    }


                    catch (SocketException ex)
                    {
                        Console.WriteLine(ex);
                        return;
                    }

                }
            
            textBox1.Text = "";
            textBox2.Text = "";
            }//ESTIC FORA DEL LOOP WHILE

        private void tirardado_Click(object sender, EventArgs e)
        {
            Random dado = new Random();
            int resultado = dado.Next(1,7);
            if (resultado == 1) 
            {
                MessageBox.Show("1");
            }
            if (resultado == 2) 
            {
                MessageBox.Show("2");
            }
            if (resultado == 3) 
            {
                MessageBox.Show("3");
            }
            if (resultado == 4) 
            {
                MessageBox.Show("4");
            }
            if (resultado == 5) 
            {
                MessageBox.Show("5");
            }
            if (resultado == 6) 
            {
                MessageBox.Show("6");
            }
        }


        }


    
}
