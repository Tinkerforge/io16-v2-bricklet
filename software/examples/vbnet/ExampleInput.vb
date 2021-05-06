Imports System
Imports Tinkerforge

Module ExampleInput
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your IO-16 Bricklet 2.0

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim io As New BrickletIO16V2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current value
        Dim value As Boolean() = io.GetValue()

        Console.WriteLine("Channel 0 [A0]: " + value(0).ToString())
        Console.WriteLine("Channel 1 [A1]: " + value(1).ToString())
        Console.WriteLine("Channel 2 [A2]: " + value(2).ToString())
        Console.WriteLine("Channel 3 [A3]: " + value(3).ToString())
        Console.WriteLine("Channel 4 [A4]: " + value(4).ToString())
        Console.WriteLine("Channel 5 [A5]: " + value(5).ToString())
        Console.WriteLine("Channel 6 [A6]: " + value(6).ToString())
        Console.WriteLine("Channel 7 [A7]: " + value(7).ToString())
        Console.WriteLine("Channel 8 [B0]: " + value(8).ToString())
        Console.WriteLine("Channel 9 [B1]: " + value(9).ToString())
        Console.WriteLine("Channel 10 [B2]: " + value(10).ToString())
        Console.WriteLine("Channel 11 [B3]: " + value(11).ToString())
        Console.WriteLine("Channel 12 [B4]: " + value(12).ToString())
        Console.WriteLine("Channel 13 [B5]: " + value(13).ToString())
        Console.WriteLine("Channel 14 [B6]: " + value(14).ToString())
        Console.WriteLine("Channel 15 [B7]: " + value(15).ToString())

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
