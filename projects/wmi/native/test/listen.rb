#!/usr/bin/ruby
require "socket"

host, port = ARGV

sock = TCPSocket.open(host, port)
bytes = 0
while data = sock.recv(100)
      data.each_byte { |byte| \
      printf "%5d%20b%15d\n", byte, byte, bytes+=1; }
end
sock.close
