/*
To guarantee the comunication, is necessary to receive and ACK from the system with the checksum/msgNum of the msg
that was delivered.

If that is not possible, and there's no feedback from the system, you could use the RS485 with UART,
so you can garantee that no other microcontroler will drive the same bus while you are transmitting data.

If both examples above are not possible, you can specify an syncronized protocol, where each
microcontroler signals each other to define which one will send the msg to the system.
*/


#define BUFFER_CNT      4
#define BUFFER_SIZE     128

typedef enum buffer_state{
    IDLE,
    PARSING,
    FULL,
}buffer_state_t;

typedef struct {
    int idx;
    int data[BUFFER_SIZE];
    buffer_state_t state;
}buffer_uart_t;

buffer_uart_t buffer[BUFFER_CNT];

static void IRQ_Uart(void);
void process_uart(buffer_uart_t*);
void check_msg(void);
void init_micro(void);
void main(void);

//interrupt vector
#pragma vector=UART0_VECTOR
static __interrupt void IRQ_Uart()
{
    unsigned char i;
    unsigned char data;
    data = UART0REG;    //read register value

    //search for an empty buffer
    for(i = 0; i < BUFFER_CNT; i++)
    {
        //finding an parsing buffer
        if(buffer[i].state == PARSING)
        {
            break;
        }
        //if doesn not exist an parsing buffer, use a new one
        else if(buffer[i].state == IDLE)
        {       
            buffer[i].state = PARSING;
            buffer[i].idx = 0;
            break;
        }
    }
    if (i == BUFFER_CNT)
    {
        return 0; //drop data. All buffers are full.
    }
    if (buffer[i].idx > BUFFER_SIZE)
    {   
        //free the buffer. Can't receive more data
        buff->state = IDLE;
        buff->idx = 0;
        return 0;
    }

    //save on the parsing buffer
    buffer[i].data[buffer[i].idx] = data;
    buffer[i].idx++;

    if(data == '\n' || (buffer[i].idx > 0x0A)) //EOT. Here can be checksum, or whenever are an EOT sintax
    {
        buffer[i].state = FULL;
    }
}

void process_data(buffer_uart_t *buff)
{
    unsigned char i;
    unsigned char check_sum;
    
    //checksum
    for(i = 0; i < buff->idx; i++)
    {
        check_sum += buff->data[i];
    }
    if(check_sum != buff->data[buff->idx])
    {
        //check sum error.
        buff->state = IDLE;
        buff->idx = 0;
        return;
    }

    //check for data
    if(buff->idx > 10) //if size bigger than 10. Seems ok
    {
        //check header
        switch (buff->data[0])
        {
            //rcv msg
            case 0x00:
                //chekc type and command
                //do whaterever you want if this type
                switch (buff->data[1])
                {
                    case 0x00:
                        //do whaterever you want if this cmd
                        break;
                }
                break;
            //send msg
            case 0x01:
                //chekc type and command
                //do whaterever you want if this type
                switch (buff->data[1])
                    {
                        case 0x00:
                            //do whaterever you want if this cmd
                            break;
                    }
                    break;
            //ack msg
            case 0x02:
                //chekc type and command
                //do whaterever you want if this type

                switch (buff->data[1])
                {
                    case 0x00:
                        //do whaterever you want if this cmd
                        break;
                }
                break;
            default:
                break;
        }
    }

    //after processing, free buffer for new data
    buff->state = IDLE;
    buff->idx = 0;
}

void check_msg(void)
{
    unsigned char i;
    for(i = 0; i < BUFFER_CNT; i++)
    {
        if(buffer[i].state == FULL)
        {
            process_data(&buffer[i])
        }
    }
}

void init_micro(void)
{
    //initialize registers and values
}

void main()
{
    init_micro();
    while(1)
    {
        check_msg();
    }
}