# Flower Exchange System (flower_proj_LSEG)

## Flower Exchange Story

The Flower Exchange System is a basic trading platform designed to facilitate the buying and selling of flowers. This system comprises two main components:

1. **Trader Application**: Traders utilize the Trader Application to submit buy or sell orders for various types of flowers. This application acts as the interface for traders to interact with the exchange.

2. **Exchange Application**: The Exchange Application serves as the core of the system. It processes incoming orders, matches them against existing orders in the Order Book, and executes orders, either fully or partially. After processing each order, the Exchange Application generates an Execution Report, which indicates the status of the order. In some cases, orders may be rejected due to factors like quantity limitations or invalid flower types.

## Installation

To set up the Flower Exchange System, follow these steps:

1. Clone the repository to your local machine:
   ```bash
   git clone https://github.com/your-username/flower_proj_LSEG.git

2. Navigate to the project directory: flower_proj_LSEG

3. Install any necessary dependencies (if applicable)

4. Launch the Trader Application and Exchange Application by executing the "lseg_reborn_new.cpp" code.

5. Customize the Order Book to your specific requirements by modifying the "order.csv" file. Observe the corresponding results meticulously recorded in the "execution_rep.csv" file.

