$root = git rev-parse --show-toplevel
$scylla = "https://github.com/x64dbg/ScyllaHide/releases/download/snapshot-2020-08-09_19-11-36/ScyllaHide.7z"

if (![System.IO.File]::Exists("$root/tools/scylla")) {
    echo "Downloading ScyllaHide"

    Invoke-WebRequest -Uri "$scylla" -OutFile ScyllaHide.7z
    7z x ScyllaHide.7z -o"$root/tools/scylla" -y
}
