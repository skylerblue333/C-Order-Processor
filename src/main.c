#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_UNIT_PRICE_CENTS UINT64_C(100000000)
#define MAX_QUANTITY UINT64_C(100000)
#define MAX_DISCOUNT_BPS UINT64_C(10000)

typedef struct {
    uint64_t unit_price_cents;
    uint64_t quantity;
    uint64_t discount_bps;
    uint64_t subtotal_cents;
    uint64_t discount_cents;
    uint64_t total_cents;
} order_quote_t;

static int parse_u64(const char *text, uint64_t *out) {
    char *end = NULL;
    unsigned long long value;

    if (text == NULL || *text == '\0' || out == NULL || *text == '-') {
        return -1;
    }
    errno = 0;
    value = strtoull(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0') {
        return -1;
    }
    *out = (uint64_t)value;
    return 0;
}

static int quote_order(uint64_t unit_price_cents, uint64_t quantity,
                       uint64_t discount_bps, order_quote_t *quote) {
    uint64_t subtotal;
    uint64_t discount;

    if (quote == NULL || unit_price_cents == 0 || unit_price_cents > MAX_UNIT_PRICE_CENTS ||
        quantity == 0 || quantity > MAX_QUANTITY || discount_bps > MAX_DISCOUNT_BPS) {
        return -1;
    }
    if (unit_price_cents > UINT64_MAX / quantity) {
        return -1;
    }

    subtotal = unit_price_cents * quantity;
    if (subtotal > UINT64_MAX / discount_bps && discount_bps != 0) {
        return -1;
    }
    discount = discount_bps == 0 ? 0 : (subtotal * discount_bps) / UINT64_C(10000);

    quote->unit_price_cents = unit_price_cents;
    quote->quantity = quantity;
    quote->discount_bps = discount_bps;
    quote->subtotal_cents = subtotal;
    quote->discount_cents = discount;
    quote->total_cents = subtotal - discount;
    return 0;
}

int main(int argc, char **argv) {
    uint64_t unit_price;
    uint64_t quantity;
    uint64_t discount_bps = 0;
    order_quote_t quote;

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "usage: %s <unit_price_cents> <quantity> [discount_bps]\n", argv[0]);
        return 2;
    }
    if (parse_u64(argv[1], &unit_price) != 0 || parse_u64(argv[2], &quantity) != 0 ||
        (argc == 4 && parse_u64(argv[3], &discount_bps) != 0) ||
        quote_order(unit_price, quantity, discount_bps, &quote) != 0) {
        fprintf(stderr, "invalid order inputs\n");
        return 2;
    }

    printf("{\"unit_price_cents\":%" PRIu64 ",\"quantity\":%" PRIu64
           ",\"discount_bps\":%" PRIu64 ",\"subtotal_cents\":%" PRIu64
           ",\"discount_cents\":%" PRIu64 ",\"total_cents\":%" PRIu64 "}\n",
           quote.unit_price_cents, quote.quantity, quote.discount_bps, quote.subtotal_cents,
           quote.discount_cents, quote.total_cents);
    return 0;
}
