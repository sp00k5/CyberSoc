FROM fauria/vsftpd

RUN sed -i.bak 's/write_enable=YES/write_enable=NO/' /etc/vsftpd/vsftpd.conf

COPY ./flag/ /home/vsftpd/EvilEddy/
RUN chown ftp:ftp /home/vsftpd/EvilEddy/flag.txt && \
	chmod 600 /home/vsftpd/EvilEddy/flag.txt


