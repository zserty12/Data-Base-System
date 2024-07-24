#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "mysql.h"
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "dnswjdrh@12A";

#define MAX_LEN 13000

using namespace std;

int main(void) {
    srand(time(NULL));
    MYSQL* connection = NULL;
    MYSQL conn;
    MYSQL_RES* sql_result;
    MYSQL_ROW sql_row;
    FILE* fp = fopen("crud_file.txt", "rt");    // open CRUD file.
    char line[MAX_LEN];

    if (mysql_init(&conn) == NULL)
        printf("mysql_init() error!");

    connection = mysql_real_connect(&conn, host, user, pw, NULL, 3306, (const char*)NULL, 0);
    if (connection == NULL) {
        printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
        return 1;
    }
    else {
        printf("Connection Succeed\n\n");

        while (fgets(line, sizeof(line), fp) != NULL) {
            if (!strcmp(line, "$$$\n"))
                break;
            mysql_query(connection, line);
        }

        if (mysql_select_db(&conn, "project")) {
            printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
            return 1;
        }

        string query;
        int state;

        while (true) {
            printf("------- SELECT QUERY TYPES -------\n\n");
            printf("\t1. TYPE 1\n");
            printf("\t2. TYPE 2\n");
            printf("\t3. TYPE 3\n");
            printf("\t4. TYPE 4\n");
            printf("\t5. TYPE 5\n");
            printf("\t6. TYPE 6\n");
            printf("\t7. TYPE 7\n");
            printf("\t0. QUIT\n");
            printf("----------------------------------\n");
            int q_num; cin >> q_num;
            if (q_num == 1) {
                printf("---- TYPE 1 ----\n\n");
                printf("Find address of homes for sale in the district “Mapo”.\n");
                query = "SELECT A.Property_address "
                    "FROM Property P "
                    "JOIN Address A ON P.Property_address = A.Property_address "
                    "JOIN District D ON A.District_name = D.District_name "
                    "WHERE D.District_name = 'Mapo';";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                if (sql_result == NULL) {
                    printf("Store Result Error: %s\n", mysql_error(connection));
                    continue;
                }
                while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    printf("%s\n", sql_row[0]);
                }
                mysql_free_result(sql_result);
                printf("\n\n");
                printf("------- SUBTYPES in TYPE1 -------\n\n");
                printf("\t1. TYPE 1-1\n");
                int sub_qnum;
                cin >> sub_qnum;
                state = 0;
                if (sub_qnum == 1) {
                    printf("---- TYPE 1-1 ----\n\n");
                    printf("Find homes costing between ₩1,000,000,000 and ₩1,500,000,000.\n");
                    query = "SELECT A.Property_address "
                        "FROM Property P "
                        "JOIN Address A ON P.Property_address = A.Property_address "
                        "JOIN District D ON A.District_name = D.District_name "
                        "WHERE D.District_name = 'Mapo' "
                        "AND P.property_price BETWEEN 1000000000 AND 1500000000;";
                    state = mysql_query(connection, query.c_str());
                    if (state != 0) {
                        printf("Query Error: %s\n", mysql_error(connection));
                        continue;
                    }
                    sql_result = mysql_store_result(connection);
                    if (sql_result == NULL) {
                        printf("Store Result Error: %s\n", mysql_error(connection));
                        continue;
                    }
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                        printf("%s\n", sql_row[0]);
                    }
                    mysql_free_result(sql_result);
                    printf("\n\n");
                }
            }
            else if (q_num == 2) {
                printf("---- TYPE 2 ----\n\n");
                printf("Find the address of homes for sale in the 8th school district.\n");
                query = "SELECT Property_address FROM Property WHERE Property_address IN (SELECT Property_address FROM Address ";
                query += "WHERE District_name IN(SELECT District_name FROM District WHERE District_ID = 8)); ";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    printf("%s\n", sql_row[0]);
                }
                mysql_free_result(sql_result);
                printf("\n\n");
                printf("------- SUBTYPES in TYPE2 -------\n\n");
                printf("\t1. TYPE 2-1\n");
                int sub_qnum; cin >> sub_qnum;
                state = 0;
                if (sub_qnum == 1) {
                    printf("---- TYPE 2-1 ----\n\n");
                    printf("Find properties with 4 or more bedrooms and 2 bathrooms.\n");
                    query = "SELECT Property_address FROM Property WHERE Property_address IN (SELECT Property_address FROM Address";
                    query += " WHERE District_name IN(SELECT District_name FROM District WHERE District_ID = 8)) AND Bedroom_num >= 4 AND Bathroom_num = 2; ";
                    state = mysql_query(connection, query.c_str());
                    if (state != 0) {
                        printf("Query Error: %s\n", mysql_error(connection));
                        continue;
                    }
                    sql_result = mysql_store_result(connection);
                    if (sql_result == NULL) {
                        printf("Store Result Error: %s\n", mysql_error(connection));
                        continue;
                    }
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                        printf("%s\n", sql_row[0]);
                    }
                    mysql_free_result(sql_result);
                    printf("\n\n");
                }
            }
            if (q_num == 3) {
                printf("---- TYPE 3 ----\n\n");
                printf("Find the name of the agent who has sold the most properties in the year 2022 by total won value.\n");
                query = "SELECT Agent_name "
                    "FROM Selling_Agent "
                    "JOIN (SELECT Agent_ID "
                    "FROM Sale "
                    "WHERE YEAR(Sold_date) = 2022 "
                    "GROUP BY Agent_ID "
                    "ORDER BY SUM(Sale_price) DESC "
                    "LIMIT 1) AS TopAgent ON Selling_Agent.Agent_ID = TopAgent.Agent_ID;";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                if (sql_result == NULL) {
                    printf("Store Result Error: %s\n", mysql_error(connection));
                    continue;
                }
                while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    printf("%s\n", sql_row[0]);
                }
                mysql_free_result(sql_result);
                printf("\n\n");
                while (1) {
                    printf("------- SUBTYPES in TYPE3 -------\n\n");
                    printf("\t1. TYPE 3-1\n");
                    printf("\t2. TYPE 3-2\n");
                    printf("\t0. TO MAIN MENU\n");
                    int sub_qnum; cin >> sub_qnum;
                    state = 0;
                    if (sub_qnum == 0) break;
                    else if (sub_qnum == 1) {
                        printf("---- TYPE 3-1 ----\n\n");
                        printf("Find the top k agents in the year 2023 by total won value.\n");
                        int k; cin >> k;
                        query = "SELECT SA.Agent_name "
                            "FROM Selling_Agent SA "
                            "JOIN (SELECT Agent_ID "
                            "FROM Sale "
                            "WHERE YEAR(Sold_date) = 2023 "
                            "GROUP BY Agent_ID "
                            "ORDER BY SUM(Sale_price) DESC "
                            "LIMIT " + to_string(k) + ") AS TopAgents "
                            "ON SA.Agent_ID = TopAgents.Agent_ID;";
                        state = mysql_query(connection, query.c_str());
                        if (state != 0) {
                            printf("Query Error: %s\n", mysql_error(connection));
                            continue;
                        }
                        sql_result = mysql_store_result(connection);
                        if (sql_result == NULL) {
                            printf("Store Result Error: %s\n", mysql_error(connection));
                            continue;
                        }
                        while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                            printf("%s\n", sql_row[0]);
                        }
                        mysql_free_result(sql_result);
                        printf("\n\n");
                    }
                    else if (sub_qnum == 2) {
                        printf("---- TYPE 3-2 ----\n\n");
                        printf("Find the bottom 10%% agents in the year 2021 by total won value.\n");

                        // Step 1: Calculate the total number of agents
                        query = "SELECT CEIL(COUNT(DISTINCT Agent_ID) * 0.1) "
                            "FROM Sale "
                            "WHERE YEAR(Sold_date) = 2021;";
                        state = mysql_query(connection, query.c_str());
                        if (state != 0) {
                            printf("Query Error: %s\n", mysql_error(connection));
                            return EXIT_FAILURE;
                        }

                        sql_result = mysql_store_result(connection);
                        if (sql_result == NULL) {
                            printf("Store Result Error: %s\n", mysql_error(connection));
                            return EXIT_FAILURE;
                        }

                        int bottom_limit = 0;
                        if ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                            bottom_limit = atoi(sql_row[0]);
                        }
                        mysql_free_result(sql_result);

                        // Step 2: Fetch bottom 10% agents by total won value in 2021
                        query = "SELECT SA.Agent_name "
                            "FROM Selling_Agent SA "
                            "JOIN (SELECT Agent_ID "
                            "      FROM Sale "
                            "      WHERE YEAR(Sold_date) = 2021 "
                            "      GROUP BY Agent_ID "
                            "      ORDER BY SUM(Sale_price) ASC "
                            "      LIMIT " + to_string(bottom_limit) + ") AS BottomAgents "
                            "ON SA.Agent_ID = BottomAgents.Agent_ID;";
                        state = mysql_query(connection, query.c_str());
                        if (state != 0) {
                            printf("Query Error: %s\n", mysql_error(connection));
                            return EXIT_FAILURE;
                        }

                        sql_result = mysql_store_result(connection);
                        if (sql_result == NULL) {
                            printf("Store Result Error: %s\n", mysql_error(connection));
                            return EXIT_FAILURE;
                        }

                        while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                            printf("%s\n", sql_row[0]);
                        }
                        mysql_free_result(sql_result);
                        printf("\n\n");
                    }
      
                }
            }
            else if (q_num == 4) {
                printf("---- TYPE 4 ----\n\n");
                printf("Compute the average selling price of properties sold in 2022, and the average time the property was on the market.\n");
                query = "SELECT AVG(Sale_price), AVG(DATEDIFF(Sold_date, For_sale_date)) FROM Sale WHERE YEAR(Sold_date) = 2022;";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                if (sql_result == NULL) {
                    printf("Store Result Error: %s\n", mysql_error(connection));
                    continue;
                }
                if ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    printf("Average Price: %s, Average Time on Market: %s days\n", sql_row[0], sql_row[1]);
                }
                mysql_free_result(sql_result);
                printf("\n\n");
                while (1) {
                    printf("------- SUBTYPES in TYPE4 -------\n\n");
                    printf("\t1. TYPE 4-1\n");
                    printf("\t2. TYPE 4-2\n");
                    printf("\t0. TO MAIN MENU\n");
                    int sub_qnum; cin >> sub_qnum;
                    state = 0;
                    if (sub_qnum == 0) break;
                    else if (sub_qnum == 1) {
                        printf("---- TYPE 4-1 ----\n\n");
                        printf("Compute the maximum selling price of properties sold in 2023 for each agent.\n");
                        query = "SELECT Agent_name, MAX(Sale_price) FROM Selling_Agent ";
                        query += "JOIN Sale ON Selling_Agent.Agent_ID = Sale.Agent_ID WHERE YEAR(Sold_date) = 2023 GROUP BY Agent_name; ";
                        state = mysql_query(connection, query.c_str());
                        if (state != 0) {
                            printf("Query Error: %s\n", mysql_error(connection));
                            continue;
                        }
                        sql_result = mysql_store_result(connection);
                        if (sql_result == NULL) {
                            printf("Store Result Error: %s\n", mysql_error(connection));
                            continue;
                        }
                        while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                            printf("Agent: %s, Max Price: %s\n", sql_row[0], sql_row[1]);
                        }
                        mysql_free_result(sql_result);
                        printf("\n\n");
                    }
                    else if (sub_qnum == 2) {
                        printf("---- TYPE 4-2 ----\n\n");
                        printf("Compute the longest time the property was on the market for each agent in 2022.\n");
                        query = "SELECT Agent_name, MAX(DATEDIFF(Sold_date, For_sale_date)) FROM Selling_Agent ";
                        query += "JOIN Sale ON Selling_Agent.Agent_ID = Sale.Agent_ID WHERE YEAR(Sold_date) = 2022 GROUP BY Agent_name; ";
                        state = mysql_query(connection, query.c_str());
                        if (state != 0) {
                            printf("Query Error: %s\n", mysql_error(connection));
                            continue;
                        }
                        sql_result = mysql_store_result(connection);
                        if (sql_result == NULL) {
                            printf("Store Result Error: %s\n", mysql_error(connection));
                            continue;
                        }
                        while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                            printf("Agent: %s, Longest Time: %s days\n", sql_row[0], sql_row[1]);
                        }
                        mysql_free_result(sql_result);
                        printf("\n\n");
                    }
                }
            }
            else if (q_num == 5) {
                printf("---- TYPE 5 ----\n\n");
                printf("Show photos of the most expensive studio, one-bedroom, multi-bedroom apartment(s), and detached house(s).\n");

                const char* query_studio =
                    "SELECT 'Studio' AS Property_type, p.Interior_photo, p.Exterior_photo, p.Floor_plan "
                    "FROM Property p "
                    "WHERE p.Property_type = 'studio' "
                    "ORDER BY p.Property_price DESC LIMIT 1;";

                const char* query_one_bedroom =
                    "SELECT 'One-bedroom' AS Property_type, p.Interior_photo, p.Exterior_photo, p.Floor_plan "
                    "FROM Property p "
                    "WHERE p.Property_type = 'one-bedroom apartment' "
                    "ORDER BY p.Property_price DESC LIMIT 1;";

                const char* query_multi_bedroom =
                    "SELECT 'Multi-bedroom' AS Property_type, p.Interior_photo, p.Exterior_photo, p.Floor_plan "
                    "FROM Property p "
                    "WHERE p.Property_type = 'multi-bedroom apartment' "
                    "ORDER BY p.Property_price DESC LIMIT 1;";

                const char* query_detached =
                    "SELECT 'Detached House' AS Property_type, p.Interior_photo, p.Exterior_photo, p.Floor_plan "
                    "FROM Property p "
                    "WHERE p.Property_type = 'detached houses' "
                    "ORDER BY p.Property_price DESC LIMIT 1;";

                struct {
                    const char* type;
                    const char* query;
                } queries[] = {
                    {"studio", query_studio},
                    {"one-bedroom apartment", query_one_bedroom},
                    {"multi-bedroom apartment", query_multi_bedroom},
                    {"detached houses", query_detached}
                };

                for (int i = 0; i < 4; i++) {
                    printf("Most expensive %s:\n", queries[i].type);
                    int state = mysql_query(connection, queries[i].query);
                    if (state != 0) {
                        printf("Query Error: %s\n", mysql_error(connection));
                        continue;
                    }
                    MYSQL_RES* sql_result = mysql_store_result(connection);
                    if (sql_result == NULL) {
                        printf("Store Result Error: %s\n", mysql_error(connection));
                        continue;
                    }
                    MYSQL_ROW sql_row;
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                        printf("Type: %s, Interior Photo: %s, Exterior Photo: %s, Floor Plan: %s\n",
                            queries[i].type, sql_row[1], sql_row[2], sql_row[3]);
                    }
                    mysql_free_result(sql_result);
                    printf("\n\n");
                }
            }
            else if (q_num == 6) {
                printf("---- TYPE 6 ----\n\n");
                printf("Record the sale of a property that had been listed as available.\n");

                // Display properties available for sale (name and address)
                query = "SELECT Property_name, Property_address FROM Property;";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                if (sql_result == NULL) {
                    printf("Store Result Error: %s\n", mysql_error(connection));
                    continue;
                }

                printf("Available Properties:\n");
                while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    printf("Property Name: %s, Address: %s\n", sql_row[0], sql_row[1]);
                }
                mysql_free_result(sql_result);

                // Get property details from user
                string property_name, property_address;
                printf("Enter the Property Name: ");
                cin.ignore();  // Clear the input buffer
                getline(cin, property_name);
                printf("Enter the Property Address: ");
                getline(cin, property_address);

                // Display buyers (ID and name)
                query = "SELECT Buyer_ID, Buyer_name FROM Buyer;";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                if (sql_result == NULL) {
                    printf("Store Result Error: %s\n", mysql_error(connection));
                    continue;
                }

                printf("Buyers:\n");
                while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    printf("Buyer ID: %s, Buyer Name: %s\n", sql_row[0], sql_row[1]);
                }
                mysql_free_result(sql_result);

                // Get buyer details from user
                int buyer_id;
                printf("Enter the Buyer ID: ");
                cin >> buyer_id;

                // Get property details from Property table
                query = "SELECT Property_price, Seller_ID, For_sale_date "
                    "FROM Property WHERE Property_name = '" + property_name +
                    "' AND Property_address = '" + property_address + "';";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                if (sql_result == NULL) {
                    printf("Store Result Error: %s\n", mysql_error(connection));
                    continue;
                }

                long long property_price = 0;
                int seller_id = 0;
                string for_sale_date;
                if ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    property_price = atoll(sql_row[0]);
                    seller_id = atoi(sql_row[1]);
                    for_sale_date = sql_row[2];
                }
                mysql_free_result(sql_result);

                // Get agent_id from Seller table using seller_id
                query = "SELECT Agent_ID FROM Seller WHERE Seller_ID = " + to_string(seller_id) + ";";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                if (sql_result == NULL) {
                    printf("Store Result Error: %s\n", mysql_error(connection));
                    continue;
                }

                int agent_id = 0;
                if ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    agent_id = atoi(sql_row[0]);
                }
                mysql_free_result(sql_result);

                // Get buyer_agent_id from Buyer table using buyer_id
                query = "SELECT Buyer_agent_ID FROM Buyer WHERE Buyer_ID = " + to_string(buyer_id) + ";";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                if (sql_result == NULL) {
                    printf("Store Result Error: %s\n", mysql_error(connection));
                    continue;
                }

                int buyer_agent_id = 0;
                if ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    buyer_agent_id = atoi(sql_row[0]);
                }
                mysql_free_result(sql_result);

                // Get current date for sold_date
                query = "SELECT DATE(NOW());";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    continue;
                }
                sql_result = mysql_store_result(connection);
                if (sql_result == NULL) {
                    printf("Store Result Error: %s\n", mysql_error(connection));
                    continue;
                }

                string sold_date;
                if ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    sold_date = sql_row[0];
                }
                mysql_free_result(sql_result);

                // Insert new tuple into Sale table
                query = "INSERT INTO Sale (Property_name, Property_address, Sold_date, Seller_ID, Agent_ID, Buyer_ID, Buyer_agent_ID, For_sale_date, Sale_price) VALUES ('" +
                    property_name + "', '" + property_address + "', '" + sold_date + "', " +
                    to_string(seller_id) + ", " + to_string(agent_id) + ", " + to_string(buyer_id) + ", " +
                    to_string(buyer_agent_id) + ", '" + for_sale_date + "', " + to_string(property_price) + ");";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Insert Error: %s\n", mysql_error(connection));
                    continue;
                }

                // Delete sold property from Property table
                query = "DELETE FROM Property WHERE Property_name = '" + property_name +
                    "' AND Property_address = '" + property_address + "';";
                state = mysql_query(connection, query.c_str());
                if (state != 0) {
                    printf("Delete Error: %s\n", mysql_error(connection));
                    continue;
                }

                printf("Property sale recorded successfully.\n\n");
                }
            else if (q_num == 7) {
                printf("---- TYPE 7 ----\n\n");
                printf("Add a new agent to the database.\n");
                string agent_name, agent_phone;
                int ID;
                bool uniqueID = false;

                printf("Enter Agent Name: ");
                cin >> agent_name;
                printf("Enter Agent Phone: ");
                cin >> agent_phone;

                // Loop until a unique ID is found
                while (!uniqueID) {
                    ID = rand() % 9999 + 1;
                    query = "SELECT COUNT(*) FROM Selling_Agent WHERE Agent_ID = " + to_string(ID) + ";";
                    state = mysql_query(connection, query.c_str());
                    sql_result = mysql_store_result(connection);
                    sql_row = mysql_fetch_row(sql_result);
                    int count = atoi(sql_row[0]);
                    mysql_free_result(sql_result);

                    if (count == 0) {
                        uniqueID = true;
                    }
                }

                query = "INSERT INTO Selling_Agent (Agent_ID, Agent_name, Agent_phone_number) VALUES (" 
                    + to_string(ID) + ", '" + agent_name + "', '" + agent_phone + "');";
                state = mysql_query(connection, query.c_str());
                if (state == 0) {
                    printf("Agent added successfully.\n");
                }
                else {
                    printf("Error adding agent: %s\n", mysql_error(&conn));
                }
                printf("\n\n");
            }
            else if (q_num == 0) {
                break;
            }
        }

        while (fgets(line, sizeof(line), fp) != NULL)
            mysql_query(connection, line);

        mysql_close(connection);
    }

    return 0;
}