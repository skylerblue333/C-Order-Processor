# Security

Sky Order Core is an engineering-beta local calculation primitive.

Inputs are parsed as bounded unsigned integers, arithmetic uses integer cents/basis points, and multiplication is checked before use. CI runs strict warnings plus AddressSanitizer and UndefinedBehaviorSanitizer contract tests. The packaged binary runs as a non-root numeric user in a scratch image.

This component does not handle card data, payment credentials, customer secrets, authentication, authorization, persistence, tax rules, network requests, or distributed order state. It is not PCI, accounting, tax, or financial-control infrastructure. Integrating systems must provide those controls independently.
