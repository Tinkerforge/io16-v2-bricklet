Imports System
Imports System.Threading
Imports Tinkerforge

Module ExampleOutput
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your IO-16 Bricklet 2.0

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim io As New BrickletIO16V2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Configure channel 7 [A7] as output low
        io.SetConfiguration(7, "o"C, False)

        ' Set channel 7 [A7] alternating high/low 10 times with 100 ms delay
        Dim i As Integer
        For i = 0 To 9
            Thread.Sleep(100)
            io.SetSelectedValue(7, True)
            Thread.Sleep(100)
            io.SetSelectedValue(7, False)
        Next i

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
