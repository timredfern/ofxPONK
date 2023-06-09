//
//  ofxPONKs.cpp
// 
//
//  Created by Tim Redfern March 2023
//

#include "ofxPONK.h"

void push8bits(std::vector<unsigned char>& fullData, unsigned char value) {
    fullData.push_back(value);
}

void push16bits(std::vector<unsigned char>& fullData, unsigned short value) {
    fullData.push_back(static_cast<unsigned char>((value>>0) & 0xFF));
    fullData.push_back(static_cast<unsigned char>((value>>8) & 0xFF));
}

void push32bits(std::vector<unsigned char>& fullData, int value) {
    fullData.push_back(static_cast<unsigned char>((value>>0) & 0xFF));
    fullData.push_back(static_cast<unsigned char>((value>>8) & 0xFF));
    fullData.push_back(static_cast<unsigned char>((value>>16) & 0xFF));
    fullData.push_back(static_cast<unsigned char>((value>>24) & 0xFF));
}

void push32bits(std::vector<unsigned char>& fullData, float value) {
    push32bits(fullData,*reinterpret_cast<int*>(&value));
}

void pushMetaData(std::vector<unsigned char>& fullData, const char (&eightCC)[9],float value) {
    for (int i=0; i<8; i++) {
        fullData.push_back(eightCC[i]);
    }
    push32bits(fullData,*(int*)&value);
}

#include <arpa/inet.h> // inet_ntop & inet_pton
#include <string.h> // strerror_r
#include <arpa/inet.h> // ntohl & htonl


string ipv4_int_to_string(uint32_t in, bool *const success)
{
    string ret(INET_ADDRSTRLEN, '\0');
    in = htonl(in);
    const bool _success = (NULL != inet_ntop(AF_INET, &in, &ret[0], ret.size()));
    if (success)
    {
        *success = _success;
    }
    if (_success)
    {
        ret.pop_back(); // remove null-terminator required by inet_ntop
    }
    else if (!success)
    {
        char buf[200] = {0};
        strerror_r(errno, buf, sizeof(buf));
        throw std::runtime_error(string("error converting ipv4 int to string ") + to_string(errno) + string(": ") + string(buf));
    }
    return ret;
}
// return is native-endian
// when an error occurs: if success ptr is given, it's set to false, otherwise a std::runtime_error is thrown.
uint32_t ipv4_string_to_int(const string &in, bool *const success)
{
    uint32_t ret;
    const bool _success = (1 == inet_pton(AF_INET, in.c_str(), &ret));
    ret = ntohl(ret);
    if (success)
    {
        *success = _success;
    }
    else if (!_success)
    {
        char buf[200] = {0};
        strerror_r(errno, buf, sizeof(buf));
        throw std::runtime_error(string("error converting ipv4 string to int ") + to_string(errno) + string(": ") + string(buf));
    }
    return ret;
}

int ofxPONKSender::draw(vector <ofPolyline> &lines,ofColor colour,int intensity){
    vector <colourPolyline> output;
    for (auto& line:lines){
        output.push_back(colourPolyline(line,colour));
    }
    return draw(output,intensity);
}

int ofxPONKSender::draw(ofPolyline &line,ofColor colour,int intensity){
    colourPolyline col=colourPolyline(line,colour);
    return draw(col,intensity);

}

int ofxPONKSender::draw(colourPolyline &line, int intensity){
    vector <colourPolyline> lines;
    lines.push_back(line);
    return draw(lines,intensity);
}

int ofxPONKSender::draw(vector <colourPolyline> &lines, int intensity){
    
    if(!enabled) return -1;

    //todo: move to a thread
    //todo: add a transform
    ofPoint output_centre(0,0);


    int xoffs=output_centre.x-(ofGetWidth()/2);
    int yoffs=output_centre.y-(ofGetHeight()/2);
    float normalised_scale=1.0f/ofGetHeight();

    return drawNormalised(lines,intensity,xoffs,yoffs,normalised_scale);
}


int ofxPONKSender::drawNormalised(vector <colourPolyline> &lines, int intensity,float x,float y,float scale){

    std::vector<unsigned char> fullData;
    fullData.reserve(65536);

    int points=0;

    float j=0.0f;
    for (auto& line:lines){
        j+=1.0f;

        fullData.push_back(PONK_DATA_FORMAT_XY_F32_RGB_U8); 
        fullData.push_back(2); //at any time?
        pushMetaData(fullData,"PATHNUMB",j);
        pushMetaData(fullData,"MAXSPEED",1.0f); //can this go BEFORE pathnumb?        
        push16bits(fullData,line.size());
        
        for (int i=0;i<line.size();i++){
            push32bits(fullData,(line[i].x+x)*scale);
            // Push Y - LSB first
            push32bits(fullData,(line[i].y+y)*scale*-1.0f);
            // Push R - LSB first
            push8bits(fullData,line.getColourAt(i).r);
            // Push G - LSB first
            push8bits(fullData,line.getColourAt(i).g);
            // Push B - LSB first
            push8bits(fullData,line.getColourAt(i).b);
            points++;
        }
        
    }

    // Compute necessary chunk count
    size_t chunksCount64 = 1 + fullData.size() / (PONK_MAX_DATA_BYTES_PER_PACKET-sizeof(GeomUdpHeader));
    if (chunksCount64 > 255) {
        ofLog()<<"Error! too many points. "<<(((PONK_MAX_DATA_BYTES_PER_PACKET*255)-sizeof(GeomUdpHeader))/11)<<" maximum";
    }

    //TODO: truncate point data at 255 chunks

    // Compute data CRC
    unsigned int dataCrc = 0;
    for (auto v: fullData) {
        dataCrc += v;
    }

    // Send all chunks to the desired IP address
    size_t written = 0;
    unsigned char chunkNumber = 0;
    unsigned char chunksCount = static_cast<unsigned char>(chunksCount64);
    while (written < fullData.size()) {
        // Write packet header - 8 bytes
        GeomUdpHeader header;
        strncpy(header.headerString,PONK_HEADER_STRING,sizeof(header.headerString));
        header.protocolVersion = 0;
        header.senderIdentifier = 123123; // Unique ID (so when changing name in sender, the receiver can just rename existing stream)
        strncpy(header.senderName,"Sample Sender",sizeof(header.senderName));
        header.frameNumber = 0; //WHAT IS THIS FOR
        header.chunkCount = chunksCount;
        header.chunkNumber = chunkNumber;
        header.dataCrc = dataCrc;

        // Prepare buffer
        std::vector<unsigned char> packet;
        size_t dataBytesForThisChunk = std::min<size_t>(fullData.size()-written, PONK_MAX_DATA_BYTES_PER_PACKET);
        packet.resize(sizeof(GeomUdpHeader) + dataBytesForThisChunk);
        // Write header
        memcpy(&packet[0],&header,sizeof(GeomUdpHeader));
        // Write data
        memcpy(&packet[sizeof(GeomUdpHeader)],&fullData[written],dataBytesForThisChunk);
        written += dataBytesForThisChunk;

        socket.sendTo(dest, &packet.front(), static_cast<unsigned int>(packet.size()));

        chunkNumber++;
    }

    //ofLog()<<"wrote "<<chunkNumber<<" chunks, total bytes "<<written;

    return points;
    /*

    

    if (device!=OFXHELIOS_NODEVICE){
        while (!dac.GetStatus(device)); //timeout for this?
            //assemble data

        vector <HeliosPoint> points;

        for (auto& line:lines){

            float dist=abs(prev_point.distance(line[0]));
            if (dist>SUBDIVIDE){
                //draw blanking points if required (only between shapes)
                for (float j=0;j<dist;j+=SUBDIVIDE){
                    float amt=j/dist;
                    points.push_back(HeliosPoint(
                        (uint16_t)(((prev_point.x*(1.0-amt))+(line[0].x*amt)+xoffs)),
                        (uint16_t)(((prev_point.y*(1.0-amt))+(line[0].y*amt)+yoffs)),
                        0,0,0,0)); //blank point
                }
            }
            for (int k=0;k<BLANK_NUM;k++){
                points.push_back(HeliosPoint(
                    (uint16_t)(line[0].x+xoffs),
                    (uint16_t)(line[0].y+yoffs),
                    0,0,0,0)); //blank point
            }
            int i;
            for (i=0;i<line.size()-1;i++){
                float dist=abs(ofPoint(line[i]).distance(ofPoint(line[i+1])));
                for (float j=0;j<dist;j+=SUBDIVIDE){
                    //draw way points
                    float amt=j/dist;
                    points.push_back(HeliosPoint(
                        (uint16_t)((line[i].x*(1.0-amt))+(line[i+1].x*amt)+xoffs),
                        (uint16_t)((line[i].y*(1.0-amt))+(line[i+1].y*amt)+yoffs),
                        (uint8_t)((((line.getColourAt(i).r*(1.0-amt))+(line.getColourAt(i+1).r*amt))*laserintensity)/255.0),
                        (uint8_t)((((line.getColourAt(i).g*(1.0-amt))+(line.getColourAt(i+1).g*amt))*laserintensity)/255.0),
                        (uint8_t)((((line.getColourAt(i).b*(1.0-amt))+(line.getColourAt(i+1).b*amt))*laserintensity)/255.0),
                        (uint8_t)intensity)
                    );
                }
                float angle=line.getDegreesAtIndex(i);
                if (angle>MAX_ANGLE||(i==line.size()-2)){
                    //dwell points to wait on a corner for laser to catch up
                    for (int l=0;l<((angle/180)*BLANK_NUM);l++){
                        points.push_back(HeliosPoint(
                            (uint16_t)(line[i+1].x+xoffs),
                            (uint16_t)(line[i+1].y+yoffs),
                            (uint8_t)(line.getColourAt(i+1).r*laserintensity/255.0),
                            (uint8_t)(line.getColourAt(i+1).g*laserintensity/255.0),
                            (uint8_t)(line.getColourAt(i+1).b*laserintensity/255.0),
                            (uint8_t)intensity)
                            );
                    }
                }
                prev_point=line[i+1];
                prev_colour=line.getColourAt(i+1);
            }
            
            for (int k=0;k<BLANK_NUM;k++){
                points.push_back(HeliosPoint(
                    (uint16_t)(prev_point.x+xoffs),
                    (uint16_t)(prev_point.y+yoffs),
                    (uint8_t)((prev_colour.r*laserintensity)/255.0),
                    (uint8_t)((prev_colour.g*laserintensity)/255.0),
                    (uint8_t)((prev_colour.b*laserintensity)/255.0),
                    (uint8_t)intensity)
                );    
            }
            
        }

        for (auto& p:points){ //hopefully avoid problems with excessive scale
            p.x=min((uint16_t)0xfff,p.x);
            p.y=min((uint16_t)0xfff,p.y);
        }

        if (HELIOS_ERROR==dac.WriteFrame(device, pps, HELIOS_FLAGS_DEFAULT, &points[0], min(HELIOS_MAX_POINTS,(int)points.size()))){
            printf("ofxHelios: write error (%i,%i,%i,%i)\n",device, pps, HELIOS_FLAGS_DEFAULT, (int)points.size());
            return -1;
        }

        return points.size();
    
            
    }
    return -2;
    */

    return 0;
}



