Author : Ishit Mehta (ibm)

How to use it:
1)exec builder.sce
2)exec loader.sce

Spawn two instances of scilab:

1)In the first instance start a server using : TcpOpen(<port>);

2)In the other instance connect to the server using : client("<ip>", <port>);


