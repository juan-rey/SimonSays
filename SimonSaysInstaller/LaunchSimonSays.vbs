Set objShell = CreateObject("WScript.Shell")
Set objFSO = CreateObject("Scripting.FileSystemObject")

' Define the name of your shortcut file exactly as it appears
strShortcutName = "SimonSays - Simply Speak.lnk"

' Get paths for both Desktop folders
strUserDesktop = objShell.SpecialFolders("Desktop") & "\" & strShortcutName
strAllUserDesktop = objShell.SpecialFolders("AllUsersDesktop") & "\" & strShortcutName

strFinalPath = ""

' Verify if and where the shortcut exists
If objFSO.FileExists(strUserDesktop) Then
    strFinalPath = objFSO.GetFile(strUserDesktop).Path
ElseIf objFSO.FileExists(strAllUserDesktop) Then
    strFinalPath = objFSO.GetFile(strAllUserDesktop).Path
End If

' If found, execute using a simplified, cleanly-quoted string
If strFinalPath <> "" Then
    
    ' We launch explorer.exe at a standard user trust level to open the shortcut.
    ' Chr(34) adds a clean single pair of double quotes around the path.
    strCommand = "cmd.exe /c runas /trustlevel:0x20000 ""explorer.exe \" & Chr(34) & strFinalPath & "\" & Chr(34) & """"
    
    ' Run hidden (0) and do not wait for completion (False)
    objShell.Run strCommand, 0, False

End If

Set objFSO = Nothing
Set objShell = Nothing
