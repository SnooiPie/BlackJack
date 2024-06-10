#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ACE 1
#define JACK 11
#define QUEEN 12
#define KING 13

const char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
const char *suitSymbols[] = {"H", "D", "C", "S"};
const char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

typedef struct {
    int rank;
    const char *suit;
    const char *suitSymbol;
} Card;

typedef struct {
    Card cards[52];
    int top;
} Deck;

void initializeDeck(Deck *deck) {
    int i, j, k = 0;
    for (i = 0; i < 4; i++) {
        for (j = 1; j <= 13; j++) {
            deck->cards[k].rank = j;
            deck->cards[k].suit = suits[i];
            deck->cards[k].suitSymbol = suitSymbols[i];
            k++;
        }
    }
    deck->top = 0;
}

void shuffleDeck(Deck *deck) {
    int i;
    for (i = 0; i < 52; i++) {
        int j = rand() % 52;
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

Card drawCard(Deck *deck) {
    return deck->cards[deck->top++];
}

int cardValue(Card card) {
    if (card.rank > 10) {
        return 10;
    }
    return card.rank;
}

void printCard(Card card) {
    printf(" ------- \n");
    printf("|%-2s     |\n", ranks[card.rank - 1]);
    printf("|       |\n");
    printf("|   %s   |\n", card.suitSymbol);
    printf("|       |\n");
    printf("|     %-2s|\n", ranks[card.rank - 1]);
    printf(" ------- \n");
}

int handValue(Card *hand, int count) {
    int value = 0, aces = 0, i;
    for (i = 0; i < count; i++) {
        if (hand[i].rank == ACE) {
            aces++;
        }
        value += cardValue(hand[i]);
    }
    while (value <= 11 && aces > 0) {
        value += 10;
        aces--;
    }
    return value;
}

void printGuide() {
    printf("Welcome to Blackjack!\n");
    printf("The objective of the game is to get as close to 21 without going over.\n");
    printf("Face cards (J, Q, K) are worth 10 points. Aces can be worth 1 or 11 points.\n");
    printf("Each player starts with two cards, and can choose to 'hit' (draw a card) or 'stand' (keep their hand).\n");
    printf("The dealer must draw cards until they have at least 17 points.\n");
    printf("Place your bet at the start of each round. Good luck!\n\n");
}

void playBlackjack() {
    int balance = 100; // Starting balance for the player
    Deck deck;
    Card playerHand[10], dealerHand[10];
    int playerCount, dealerCount;
    char choice;
    int bet;

    while (1) {
        printf("1. Start Game\n");
        printf("2. View Guide\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                initializeDeck(&deck);
                shuffleDeck(&deck);


                while (balance > 0) {
                    printf("Your current balance: $%d\n", balance);
                    printf("Enter your bet: ");
                    scanf("%d", &bet);

                    if (bet > balance) {
                        printf("You cannot bet more than your current balance!\n");
                        continue;
                    }

                    playerCount = dealerCount = 0;
                    playerHand[playerCount++] = drawCard(&deck);
                    playerHand[playerCount++] = drawCard(&deck);
                    dealerHand[dealerCount++] = drawCard(&deck);
                    dealerHand[dealerCount++] = drawCard(&deck);

                    printf("Dealer's Hand:\n");
                    printCard(dealerHand[0]);
                    printf(" [Hidden Card]\n");

                    printf("\nYour Hand:\n");
                    for (int i = 0; i < playerCount; i++) {
                        printCard(playerHand[i]);
                    }

                    while (1) {
                        int playerValue = handValue(playerHand, playerCount);
                        if (playerValue > 21) {
                            printf("\nYour hand value: %d. You busted!\n", playerValue);
                            balance -= bet;
                            break;
                        }

                        printf("\nYour hand value: %d\n", playerValue);
                        printf("Hit or Stand? (h/s): ");
                        scanf(" %c", &choice);

                        if (choice == 'h') {
                            playerHand[playerCount++] = drawCard(&deck);
                            printf("\nYour Hand:\n");
                            for (int i = 0; i < playerCount; i++) {
                                printCard(playerHand[i]);
                            }
                        } else {
                            break;
                        }
                    }

                    if (handValue(playerHand, playerCount) <= 21) {
                        printf("\nDealer's Hand:\n");
                        for (int i = 0; i < dealerCount; i++) {
                            printCard(dealerHand[i]);
                        }

                        while (handValue(dealerHand, dealerCount) < 17) {
                            dealerHand[dealerCount++] = drawCard(&deck);
                            printf("\nDealer draws a card:\n");
                            for (int i = 0; i < dealerCount; i++) {
                                printCard(dealerHand[i]);
                            }
                        }

                        int playerValue = handValue(playerHand, playerCount);
                        int dealerValue = handValue(dealerHand, dealerCount);

                        printf("\nYour hand value: %d\n", playerValue);
                        printf("Dealer's hand value: %d\n", dealerValue);

                        if (dealerValue > 21 || playerValue > dealerValue) {
                            printf("You win!\n");
                            balance += bet;
                        } else if (playerValue < dealerValue) {
                            printf("Dealer wins!\n");
                            balance -= bet;
                        } else {
                            printf("It's a tie!\n");
                        }
                    }

                    if (balance <= 0) {
                        printf("You are out of money. Game over!\n");
                        break;
                    }

                    printf("Play another round? (y/n): ");
                    scanf(" %c", &choice);
                    if (choice != 'y') {
                        break;
                    }
                }
                break;

            case '2':
                printGuide();
                break;

            case '3':
                printf("Goodbye!\n");
                exit(0);

            default:
                printf("Invalid option. Please choose again.\n");
        }
    }
}

int main() {
    srand(time(NULL));
    playBlackjack();
    return 0;
}
