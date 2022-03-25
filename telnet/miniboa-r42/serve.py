from miniboa import TelnetServer
import subprocess
CLIENT_LIST = []
SERVER_RUN = True
def on_connect(client):
    print ("++ Opened connection to %s") % client.addrport()
    CLIENT_LIST.append(client)
    client.send("Your options are:\n 1. Check a server by typing an IP address\n 2. Quit by typing quit\n")

def process_clients():
    for client in CLIENT_LIST:
        if client.active and client.cmd_ready:
            cmd(client)


def cmd(client):
    global SERVER_RUN
    msg = client.get_command()
    cmd = msg.lower()
    if cmd == 'quit':
        client.active = False
    else:
        output = subprocess.Popen(["host %s" % cmd], stdout=subprocess.PIPE, shell=True).communicate()[0]
        client.send(output)

if __name__ == '__main__':
    telnet_server = TelnetServer(
        port=7777,
        address='',
        on_connect=on_connect,
        timeout = .05
    )
    print(">> Listening for connections on port %d. CTRL-C to break." % telnet_server.port)
    while SERVER_RUN:
        telnet_server.poll()
        ## Send, Recv, and look for new connections
        process_clients()
        ## Check for client input
        #print(">> Server shutdown.")
