Imports System
Imports Tinkerforge

Module ExampleInterrupt
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your IO-16 Bricklet 2.0

    ' Callback subroutine for input_value callback
    Sub InputValueCB(ByVal sender As BrickletIO16V2, ByVal channel As Byte, _
                     ByVal changed As Boolean, ByVal value As Boolean)
        Console.WriteLine("Channel: " + channel.ToString())
        Console.WriteLine("Changed: " + changed.ToString())
        Console.WriteLine("Value: " + value.ToString())
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim io As New BrickletIO16V2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register input_value callback to subroutine InputValueCB
        AddHandler io.InputValueCallback, AddressOf InputValueCB

        ' Configure callback for channel 4 with fixed 500ms period
        io.SetInputValueCallbackConfiguration(4, 500, False)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
