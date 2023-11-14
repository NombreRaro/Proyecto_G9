using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ejercicio_individual
{
    public partial class Form2 : Form
    {
        string username, password;
        public Form2()
        {
            InitializeComponent();
            this.username = "";
            this.password = "";
            pictureBox1.BackgroundImageLayout = ImageLayout.Stretch;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.username = textBox1.Text;
            this.password = textBox2.Text;
            this.Close();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void Registre_Click(object sender, EventArgs e)
        {

        }

        public String getCreds()
        {
            return (this.username + ":" + this.password);
        }
    }
}
