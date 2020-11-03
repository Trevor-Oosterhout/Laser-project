#ifndef FUNCTIE_HPP
#define FUNCTIE_HPP

uint16_t createmessage(uint16_t playernumber, uint16_t data){
    uint16_t msg = 0;
    uint16_t controlebits = 0;
    uint16_t playerbits = 0;
    uint16_t databits = 0;
    controlebits = playernumber ^ data;
    playerbits = playernumber << 10;
    databits = data << 5;
    msg = msg | controlebits;
    msg = msg | playerbits;
    msg = msg | databits;
    return msg;
}

#endif // FUNCTIE_HPP
