#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* An array of balances in accounts, indexed by account number. */

float* account_balances;

/* Transfer DOLLARS from account FROM_ACCT to account TO_ACCT. Return
 0 if the transaction succeeded, or 1 if the balance FROM_ACCT is
 too small. */

int process_transaction (int from_acct, int to_acct, float dollars)
{
    int old_cancel_state;

    /* Check the balance in FROM_ACCT. */
    if (account_balances[from_acct] < dollars)
        return 1;

    /* Begin critical section. */
    pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_cancel_state);

    /* Move the money. */
    account_balances[to_acct] += dollars;
    account_balances[from_acct] -= dollars;

    /* End critical section. */
    pthread_setcancelstate (old_cancel_state, NULL);
    return 0;
} 

/*Main agregado*/
int main()
{
    int num_accounts = 3;
    account_balances = (float*)malloc(sizeof(float) * num_accounts);

    /* Inicializamos balances */
    account_balances[0] = 100.0;
    account_balances[1] = 200.0;
    account_balances[2] = 300.0;

    printf("Balances iniciales:\n");
    for (int i = 0; i < num_accounts; i++)
        printf("Cuenta %d: %.2f\n", i, account_balances[i]);

    /* Hacemos algunas transacciones */
    if (process_transaction(0, 1, 50.0) == 0)
        printf("\nTransacción 0->1 de $50 exitosa\n");
    else
        printf("\nTransacción 0->1 de $50 fallida\n");

    if (process_transaction(2, 0, 400.0) == 0)
        printf("Transacción 2->0 de $400 exitosa\n");
    else
        printf("Transacción 2->0 de $400 fallida\n");

    printf("\nBalances finales:\n");
    for (int i = 0; i < num_accounts; i++)
        printf("Cuenta %d: %.2f\n", i, account_balances[i]);

    free(account_balances);
    return 0;
}