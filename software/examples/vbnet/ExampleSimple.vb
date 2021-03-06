Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Analog Out Bricklet 3.0

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim ao As New BrickletAnalogOutV3(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Set output voltage to 3.3V
        ao.SetOutputVoltage(3300)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
