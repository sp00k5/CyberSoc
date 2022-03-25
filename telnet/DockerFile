FROM debian:stable
RUN apt-get update && apt-get install -y --force-yes python sudo wget netcat
RUN wget https://bootstrap.pypa.io/pip/2.7/get-pip.py
RUN python2 get-pip.py
RUN pip install miniboa

RUN adduser --disabled-password --gecos '' mike
RUN echo 'mike ALL=(phillip) NOPASSWD:/bin/bash' >> /etc/sudoers

RUN adduser --disabled-password --gecos '' phillip

COPY ./flag.enc /etc/
ADD miniboa.tar.gz /home/mike/

RUN chown phillip:phillip /etc/flag.enc
RUN chmod 700 /etc/flag.enc

USER mike
WORKDIR /home/mike
ENTRYPOINT ["python", "/home/mike/miniboa-r42/serve.py"]
