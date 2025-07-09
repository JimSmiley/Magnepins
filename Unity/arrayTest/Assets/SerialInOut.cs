using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System.Threading;
using System;
using TMPro;



public class SerialInOut : MonoBehaviour
{
   
    public string COM = "COM5";
   
 
    SerialPort sp;
    Thread ReadThread;
    string s;
   
    void Start()
    {
        
        sp = new SerialPort(COM, 115200);
        sp.ReadTimeout = 5000;
        sp.WriteTimeout = 5000;
       // sp.WriteTimeout = 1000;
        sp.Parity = Parity.None;
        sp.DataBits = 8;
        sp.StopBits = StopBits.One;
        sp.RtsEnable = true;
        sp.DtrEnable = true;
        // serialPort.RtsEnable = true;
        sp.Handshake = Handshake.None;
        sp.Encoding = System.Text.Encoding.UTF8;
        sp.NewLine = "\n";  // Need this or ReadLine() fails
  
   
        try
        {
            sp.Open();
        }
        catch (SystemException f)
        {
            print("FAILED TO OPEN PORT");

        }
        if (sp.IsOpen)
        {
            print("ConfigOutput Serial Running");

           ReadThread = new Thread(new ThreadStart(ReadSerial));
            ReadThread.Start();
           // StartCoroutine(WriteConfigLine());
        }
    }
    void ReadSerial()
    {
        while (ReadThread.IsAlive)
        {
            try
            {
                if (sp.BytesToRead > 1)
                {


                    string indata = sp.ReadLine();
                    Debug.Log(indata);  
                                     
                  


                }
            }
            catch (SystemException f)
            {
               // print(f);
                ReadThread.Abort();
            }

        }
    }

    public void WriteSerial(int[,] vals, bool spacing) // write the lines one by one as arduino has small serial buffer
    {
       if (spacing)
        {
            s = "b";
        }
       else
        {
            s = "t";
        }
        
        for (int i = 0; i < vals.GetLength(1); i++)
        {
            
            for (int j = 0; j < vals.GetLength(0); j++)
            {
                //print(vals[j, i].ToString());
                    s = s+ vals[j, i].ToString();
                                               // sp.Write(vals[j,i].ToString());




            }

           // Debug.Log(s);
            try
            {
            //   sp.WriteLine(s);
                //  Debug.Log(s);
            }
            catch (SystemException f)
            {
                print(f.ToString());
            }

           // s = "";
        }
        
        try
        {
            sp.WriteLine(s);
           // Debug.Log(s);
        }
        catch (SystemException f)
        {
            print(f.ToString());
        }
        

      
        print(s);


    }
   public void WriteTestString(string s)
    {
        try
        {
            sp.WriteLine(s);
            Debug.Log(s);
        }
        catch (SystemException f)
        {
            print(f.ToString());
        }

    }




    void OnApplicationQuit()
        {
            ReadThread.Abort();
        }
    }
