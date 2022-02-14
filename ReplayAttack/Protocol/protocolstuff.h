#ifndef PROTOCOLSTUFF_H
#define PROTOCOLSTUFF_H

namespace replayattack {

enum CommandCode {Unknown      = 0x00,
                  StartSession = 0x01,
                  UploadImage  = 0x02,
                  EndSession   = 0x03};

enum ResultCode {ImageUpdated    = 0x01,
                 SessionPrepared = 0x02,
                 SessionFinished = 0x03,
                 InvalidImage    = 0x04};

}

#endif // PROTOCOLSTUFF_H
