# Commands    

List of useful command: 

```
find / -perm /6000 -type f -executable 2>/dev/null 
```
> Useful for finding all executable  binaries with execute permission to user account logged in as   

```
sudo update-alternatives --set iptables /usr/sbin/iptables-legacy
sudo update-alternatives --set ip6tables /usr/sbin/ip6tables-legacy
sudo dockerd &
```
> Activating docker on debian WSL 2   

```
ffmpeg -i input.mp4 -vf "fps=10,scale=600:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" -loop 0 output.gif
```
> create a GIF in bash using .mp4   

```
Get-Content .\1.txt | Select-String "name" 

Select-String -Path "Users\*.txt" -Pattern "name"
```
> powershell search 1 or all .txt file for name   

```
bcdedit /set hypervisorlaunchtype auto
```
> Run this to turn on WSL but can't have Oracle VM (off for ORACLE can't use WSL)   

```
var x = document.querySelectorAll("a");
var myarray = []
for (var i=0; i<x.length; i++){
var nametext = x[i].textContent;
var cleantext = nametext.replace(/\s+/g, ' ').trim();
var cleanlink = x[i].href;
myarray.push([cleantext,cleanlink]);
};
function make_table() {
    var table = '<table><thead><th>Name</th><th>Links</th></thead><tbody>';
   for (var i=0; i<myarray.length; i++) {
            table += '<tr><td>'+ myarray[i][0] + '</td><td>'+myarray[i][1]+'</td></tr>';
    };
 
    var w = window.open("");
document.write(table); 
}
make_table()
```
> Show all links on webpage (in console)   

```
git log --all --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit
```
> Create a git workflow diagram in terminal showing stuff   

   