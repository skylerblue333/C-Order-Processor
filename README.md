# Sky Order Core

**Status: engineering beta.** A deterministic C11 order-total calculation primitive using integer cents and basis points.

## Implemented

```bash
./app <unit_price_cents> <quantity> [discount_bps]
```

- Unit price: 1–100,000,000 cents.
- Quantity: 1–100,000.
- Discount: 0–10,000 basis points (0–100%).
- Integer-only subtotal, discount, and total calculation.
- Explicit multiplication overflow checks.
- JSON output for quote fields.
- Strict C11 warnings-as-errors build, contract tests, ASan/UBSan verification, CMake build, and non-root scratch container.

Example:

```bash
./app 2500 2 1000
# {"unit_price_cents":2500,"quantity":2,"discount_bps":1000,"subtotal_cents":5000,"discount_cents":500,"total_cents":4500}
```

## Scope limitations

This component calculates a local deterministic quote only. It does **not** create or persist orders, authorize or capture payments, calculate jurisdictional tax, manage inventory, shipping, refunds, invoices, discounts from external policy, customer identity, fraud controls, idempotency across requests, HA, or production deployment.

The arithmetic should not be treated as accounting, tax, or payment-system certification.

## SKYCOIN4444 integration

Use the calculation boundary where a small deterministic pre-order total is useful. Authoritative catalog prices, tax, inventory, payment authorization, persistence, and audit records must be provided by separately verified systems.
