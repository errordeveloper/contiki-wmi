#!/usr/bin/ruby
require "socket"
 
host, port = ARGV

s = TCPSocket.open(host, port)
b = 0
while char = s.recv(100)
char.each_byte { |byte| puts "#{byte.to_i }\t\t#{byte.to_i.to_s(2)}\t#{(b+=1).to_s}" }
end
s.close
