testTcpConnection : ../Channel.* ../Socket.* ../Poller.* ../Eventloop.* ../Buffer.* ../TcpConnection.* testTcpConnection.cpp ../logger.* 
	g++ ../Channel.* ../Socket.* ../Poller.* ../Eventloop.* ../Buffer.* ../TcpConnection.* testTcpConnection.cpp ../logger.* -o testTcpConnection
testServer :   ../Acceptor.* ../Channel.* ../Socket.* ../Poller.* ../Eventloop.* ../Buffer.* ../TcpConnection.* testServer.cpp ../logger.* ../Server.* ../defaultCallBacks.* ../ThreadPool.*
	g++ ../Channel.* ../Acceptor.* ../Socket.* ../Poller.* ../Eventloop.* ../Buffer.* ../TcpConnection.* testServer.cpp ../logger.* ../Server.* ../defaultCallBacks.* ../ThreadPool.* -o testServer -lpthread

pp_server :   ../Acceptor.* ../Channel.* ../Socket.* ../Poller.* ../Eventloop.* ../Buffer.* ../TcpConnection.* pp_server.cpp ../logger.* ../Server.* ../defaultCallBacks.* ../ThreadPool.*
	g++ ../Channel.* ../Acceptor.* ../Socket.* ../Poller.* ../Eventloop.* ../Buffer.* ../TcpConnection.* pp_server.cpp ../logger.* ../Server.* ../defaultCallBacks.* ../ThreadPool.* -o pp_server -lpthread