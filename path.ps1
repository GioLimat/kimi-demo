$kmiPath = "$env:USERPROFILE\Tools\kimi\bin"

$oldPath = [Environment]::GetEnvironmentVariable("Path", "User")

if ($oldPath -notlike "*$kmiPath*") {
    $newPath = "$oldPath;$kmiPath"
    [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
    Write-Host "Kimi added to Path variables!"
} else {
    Write-Host "Kimi already is in your machine."
}