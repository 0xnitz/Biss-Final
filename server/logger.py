import logging


class Logger(object):

    def __init__(self, name='logger', level=logging.DEBUG):
        self.logger = logging.getLogger(name)
        self.logger.setLevel(level)

        fh = logging.FileHandler('%s.log' % name, 'a')
        self.logger.addHandler(fh)

        sh = logging.StreamHandler()
        self.logger.addHandler(sh)

    def info(self, msg):
        self.logger.info(msg)


if __name__ == '__main__':
    log = Logger()
    log.info('info')
