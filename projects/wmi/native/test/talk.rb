#!/usr/bin/ruby
require "socket"

host, port = ARGV


msg = ""

(0x01..0x0f).each { |x| msg << x }

msg << '\n'

sock = TCPSocket.open(host, port)
bytes = 0
sock.send(msg, 0)
sock.close
