# Cache conherence

在一个共享缓存模型的具有私有缓存的多核处理器当中，缓存一致性（cache conherence）是一个重要问题。当其中一个核对共享缓存修改之后，如何保持缓存一致性是一个重要的策略。

## 一致性策略

### Snooping

Snooping，窥探，是Bus snooping（总线窥探）的简称，是一种重要的缓存一致性策略， 在1983年就已经提出。当共享缓存中一个特定的数据块被修改时，这个改动必须传播到所有共享这个数据块的处理器的私有缓存当中。数据改动可以被总线窥探的技术所留意，snooper监测总线中的所有事务，如果一个事务要修改其中一个共享缓存块，那么所有的snooper就会检查它们的私有缓存中是否有该数据块的拷贝，如果有则更新该数据块。在这种策略下，每个cache unit都会有一个snoop unit。

#### Snooping protocols

##### Write-invalidate

如果一个处理器核写一个共享缓存数据块时，所有共享该缓存数据块的私有缓存将该缓存区块设置为无效。

##### Write-update

如果一个处理器核写一个共享缓存数据块时，所有共享该缓存数据块的私有缓存将更新该缓存区块。

### Directory-based

Directory-base，基于字典的缓存一致性策略，是指使用字典来管理缓存。与Snooping的不同在于它具有可扩展性。

https://ithelp.ithome.com.tw/articles/10158488
