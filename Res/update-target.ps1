Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing


$RootSourceDir = $args[0]
$SourceHeaders = Join-Path $RootSourceDir "include"
$Project = $args[1]
$ConfigDir  = $args[2]
$DestinationHeadersDir = Join-Path $args[3] $Project
$BinDevDestinationDir = Join-Path $args[3] $Project $ConfigDir
$BinDestinationDir = "J:\Work\Programs"
$BinariesSourceDir = Join-Path $RootSourceDir $ConfigDir

#$form = New-Object System.Windows.Forms.Form
#$form.Text = 'Évenement post-build:'
#$form.Size = New-Object System.Drawing.Size(300,200)
#$form.StartPosition = 'CenterScreen'

#$okButton = New-Object System.Windows.Forms.Button
#$okButton.Location = New-Object System.Drawing.Point(75,120)
#$okButton.Size = New-Object System.Drawing.Size(75,23)
#$okButton.Text = 'Oui'
#$okButton.DialogResult = [System.Windows.Forms.DialogResult]::OK
#$form.AcceptButton = $okButton
#$form.Controls.Add($okButton)

#$cancelButton = New-Object System.Windows.Forms.Button
#$cancelButton.Location = New-Object System.Drawing.Point(150,120)
#$cancelButton.Size = New-Object System.Drawing.Size(75,23)
#$cancelButton.Text = 'Non'
#$cancelButton.DialogResult = [System.Windows.Forms.DialogResult]::Cancel
#$form.CancelButton = $cancelButton
#$form.Controls.Add($cancelButton)

#$label = New-Object System.Windows.Forms.Label
#$label.Location = New-Object System.Drawing.Point(10,20)
#$label.Size = New-Object System.Drawing.Size(280,20)
#$label.Text = 'Mettre-à-jour ?'
#$form.Controls.Add($label)

#$textBox = New-Object System.Windows.Forms.TextBox
#$textBox.Location = New-Object System.Drawing.Point(10,40)
#$textBox.Size = New-Object System.Drawing.Size(260,20)
#$form.Controls.Add($textBox)

#$form.Topmost = $true

$SourceDir = Join-Path $RootSourceDir "include"
#$form.Add_Shown({$textBox.Select()})
#$result = $form.ShowDialog()

#if ($result -eq [System.Windows.Forms.DialogResult]::OK)
#{
	Write-Host "Procède à la copie des headers:"  $SourceDir
	Write-Host "vers: " $DestinationHeadersDir
	Write-Host "Procède à la copie des binaires: " $BinariesSourceDir
	Write-Host "vers: " $BinDestinationDir
	Write-Host " test: 1: fichier dll:" $BinariesSourceDir\$Project.dll
	Write-Host " test: 2: destination fichier dll:" $BinDestinationDir
	
	Write-Host "Headers"
	Copy-Item -Path $SourceHeaders -Destination $DestinationHeadersDir -recurse -Force
	
	Write-Host "Copie de la DLL"
	New-Item -ItemType File -Path $BinDestinationDir\$Project.dll -Force | out-null
	Copy-Item -Path $BinariesSourceDir\$Project.dll  -Destination $BinDestinationDir -Force
	
	New-Item -ItemType File -Path $BinDevDestinationDir\$Project.lib -Force | out-null
	Copy-Item -Path $BinariesSourceDir\$Project.lib  -Destination $BinDevDestinationDir -Force
	
#}

