Public Class Form1

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Dim devs() As NetPcap.Device = NetPcap.Device.GetAllNetworkInterfaces()

        For Each dev As NetPcap.Device In devs
            Me.ComboBoxDevices.Items.Add(dev)
        Next

        Me.ComboBoxDevices.SelectedIndex = 0
    End Sub

    Private Sub ButtonOpenDevice_Click(sender As Object, e As EventArgs) Handles ButtonOpenDevice.Click
        Dim device As NetPcap.Device = DirectCast(Me.ComboBoxDevices.SelectedItem, NetPcap.Device)
        Dim packet As NetPcap.Packet

        device.Open()

        While (1)
            Dim result As Integer = device.NextPacket(packet)
            Dim data() As Byte = packet.Data

            If (packet.CaptureLength < 1) Then
                MsgBox("Result: " & result & vbCrLf & "Length: " & packet.Length & vbCrLf & "Capture length: " & packet.CaptureLength)
            Else
                MsgBox("Result: " & result & vbCrLf & "Length: " & packet.Length & vbCrLf & "Capture length: " & packet.CaptureLength & vbCrLf & vbCrLf & System.Text.Encoding.ASCII.GetString(packet.Data))
            End If
        End While
        

    End Sub

    Private Sub ComboBoxDevices_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ComboBoxDevices.SelectedIndexChanged
        Dim device As NetPcap.Device = DirectCast(Me.ComboBoxDevices.SelectedItem, NetPcap.Device)

        If (device.IsOpen) Then
            ButtonOpenDevice.Enabled = False
        Else
            ButtonOpenDevice.Enabled = True
        End If
    End Sub
End Class
