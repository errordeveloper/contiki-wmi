require "socket"
 
host, port = ARGV

s = TCPSocket.open(host, port)
while char = s.recv(1)
	char.each_byte { |byte| puts "#{byte.to_i }\t\t#{byte.to_i.to_s(2)}" }
	#char.each_byte { |byte| p Array(byte.to_i).pack("N*").unpack("L*").to_s }
	#p Array(char.to_i).pack("L*").to_s
end
s.close
